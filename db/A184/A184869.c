/*
 * A184869  Numbers k such that floor[(k-1/2)*(2+2^(1/2))+1/2] is prime.
 * https://oeis.org/A184869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that floor[(k-1/2)*(2+sqrt(2))+1/2] is prime. This
 *            implementation finds k by testing candidates and caching the
 *            resulting terms
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 1157, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9318 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cache for computed terms.  The cache grows as needed. */
static seq_t *seq_cache  = NULL;
static size_t cache_size   = 0;   /* number of terms stored */
static size_t cache_capacity = 0;
static idx_t next_k_global = 1;   /* next k to test for membership */

/* Ensure at least n terms have been computed and cached. */
static void ensure_terms(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Expand cache capacity if necessary. */
    if (cache_capacity < n) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
        if (!tmp)
            exit(EXIT_FAILURE);
        seq_cache = tmp;
        cache_capacity = new_cap;
    }

    /* Generate terms until we have n of them. */
    while (cache_size < n) {
        seq_t candidate;
        while (true) {
            candidate = (seq_t)next_k_global++;
            /* d = 2*k - 1  (odd number). */
            seq_t d = candidate * 2 - 1;
            /* floor( d * sqrt(2) ) = floor_sqrt( 2 * d * d ). */
            seq_t t = floor_sqrt((d * d) * 2);
            /* round( d * sqrt(2) / 2 ) = (t + 1) / 2. */
            seq_t round_mid = (t + 1) >> 1;
            /* m = 2*k - 1 + round_mid. */
            seq_t m = candidate * 2 - 1 + round_mid;
            if (is_prime_big(m))
                break;   /* candidate satisfies the definition */
        }
        seq_cache[cache_size++] = candidate;
    }
}

/* Public function that returns the nth term of the sequence
   (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset 1; callers should not request 0 */
    ensure_terms(n);
    return seq_cache[n - 1];
}

/*
 * A088709  Numbers m that are a product of two primes j and k such that m+j+k
 *          is prime.
 * https://oeis.org/A088709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer m that is a product of two primes j and
 *            k such that m + j + k is prime. Computed by a simple search
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 963 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* External types supplied by the build */
typedef idx_t idx_t;   /* defined elsewhere */
typedef seq_t seq_t;   /* defined elsewhere */

/* Cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Current value to test for semiprime */
static uint64_t current_m = 6;   /* first possible semiprime */

/* Grow the cache when needed */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    } else {
        cache_cap *= 2;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
}

/* Test whether a uint64_t value is a product of exactly two primes and
   whether the sum with its prime factors is prime.                          */
static bool candidate(uint64_t m, uint64_t *p_out, uint64_t *q_out)
{
    /* Find smallest prime factor p of m (trial division).  If none is found,
       m is prime.  */
    uint64_t p = 0;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            p = d;
            break;
        }
    }
    if (p == 0)          /* m is prime */
        return false;

    uint64_t q = m / p;
    if (!is_prime_big((seq_t)q))
        return false;    /* not a semiprime with two primes */

    /* Check if m + p + q is prime.  Use 128‑bit arithmetic for safety. */
    unsigned __int128 sum128 = (unsigned __int128)m + p + q;
    seq_t sum = (seq_t)sum128;
    if (!is_prime_big(sum))
        return false;

    *p_out = p;
    *q_out = q;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but guard */

    /* Ensure cache has enough space for n terms */
    if (cache_len < n) {
        if (cache_cap < n) {
            while (cache_cap < n)
                grow_cache();
        }

        /* Generate new terms until cache contains n elements */
        while (cache_len < n) {
            uint64_t p, q;
            /* Find next candidate */
            while (true) {
                uint64_t m = current_m++;
                if (m < 6)          /* skip numbers < 6 (no semiprime there) */
                    continue;
                if (candidate(m, &p, &q)) {
                    cache[cache_len++] = (seq_t)m;
                    break;          /* move on to next required term */
                }
            }
        }
    }

    return cache[n - 1];
}

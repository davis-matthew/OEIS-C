/*
 * A184863  Numbers k such that floor(n+nr-r/2) are prime, where
 *          r=(1+sqrt(5))/2.
 * https://oeis.org/A184863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that floor(k*phi^2 - phi/2) is
 *            prime, where phi = (1+sqrt(5))/2. This is equivalent to
 *            floor((6k-1)+(2k-1)*sqrt(5))/4 being prime
 * Range:     u8 none, u16 n <= 6, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 92 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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

seq_t a(idx_t n) {
    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_k = 1;       /* next integer k to test (1‑based) */

    /* Grow cache until it contains the requested term. */
    while (cache_len < (size_t)n) {
        /* Use the exact formula
           f(k) = floor((6k-1)+(2k-1)*sqrt(5)) / 4. */
        seq_t k = (seq_t)next_k;

        seq_t a_val = ((seq_t)6 * k) - 1;        /* a = 6k-1  */
        seq_t b_val = ((seq_t)2 * k) - 1;        /* b = 2k-1  */

        /* temp = 5*b*b, whose square root floor is floor(b*sqrt(5)) */
        seq_t temp = ((seq_t)5 * b_val) * b_val;

        seq_t floor_b = floor_sqrt(temp);      /* floor(b*sqrt(5)) */

        seq_t sum = a_val + floor_b;           /* a + floor(b*sqrt(5)) */

        /* t is sum mod 4; we can use masking because 4 is a power of two. */
        seq_t t = sum & 3;                     /* 0 <= t <= 3 and (sum-t) divisible by 4 */

        seq_t m = (sum - t) >> 2;             /* floor((a+floor(b*sqrt(5)))/4) */

        /* If the value is prime, store the corresponding k. */
        if (is_prime_big(m)) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)next_k);
        }

        ++next_k; /* test next integer */
    }

    /* n is 1‑based, but the array is 0‑based. */
    return cache[(size_t)n - 1];
}

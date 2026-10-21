/*
 * A224573  Number of (n+5)X9 0..1 matrices with each 6X6 subblock idempotent.
 * https://oeis.org/A224573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the 16‑term recurrence a(n) = 3*a(n-1) -4*a(n-2)
 *            +4*a(n-3) -3*a(n-4) +a(n-5) +2*a(n-6) -5*a(n-7) +6*a(n-8)
 *            -6*a(n-9) +4*a(n-10) -a(n-11) -a(n-12) +2*a(n-13) -2*a(n-14)
 *            +2*a(n-15) -a(n-16) with initial values a(1)=18868, a(2)=6411,
 *            …, a(20)=161005
 * Range:     u8 none, u16 n <= 16, u32 n <= 60, u64 n <= 149, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* off‑by‑one guard, not used */

    /* first 20 terms (offset 1) */
    static const seq_t init[20] = {
        18868, 6411, 6659, 7394, 8143,
        8714, 9164, 10807, 13624, 17332,
        21731, 26592, 31785, 38491, 47899,
        60907, 78205, 100219, 127312, 161005
    };

    /* recurrence coefficients for k = 1..16 (a(n-k)) */
    static const int coeff[17] = {
        0,  /* unused 0‑th element */
        3, -4,  4, -3,  1,  2, -5,  6, -6,  4, -1, -1,  2, -2,  2, -1
    };

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of stored terms */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 20;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        memcpy(cache, init, cache_cap * sizeof(seq_t));
        cache_len = cache_cap;
    }

    /* grow cache if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure (unlikely) */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute new terms until we reach index n */
    while (cache_len < (size_t)n) {
        idx_t idx = (idx_t)cache_len + 1;           /* next n (1‑based) */
        seq_t sum = 0;
        for (int k = 1; k <= 16; ++k) {
            seq_t term = cache[idx - 1 - k];        /* a(idx-k) */
            int c = coeff[k];
            if (c > 0) sum += (seq_t)c * term;
            else       sum -= (seq_t)(-c) * term;
        }
        cache[cache_len] = sum;
        cache_len++;
    }

    return cache[(size_t)n - 1];
}

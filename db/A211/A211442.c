/*
 * A211442  Number of (n+1) X (n+1) -7..7 symmetric matrices with every 2 X 2
 *          subblock having sum zero and three distinct values.
 * https://oeis.org/A211442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:  a(n) = 4*a(n-1) + 5*a(n-2) - 36*a(n-3) + 6*a(n-4)
 *                       + 119*a(n-5) - 71*a(n-6) - 180*a(n-7) + 134*a(n-8)
 *                       + 130*a(n-9) - 90*a(n-10) - 48*a(n-11) + 20*a(n-12)
 *                       + 8*a(n-13);
 * Initial values: a(1)=90,a(2)=188,a(3)=356,a(4)=654,a(5)=1190,a(6)=2144,
 *                 a(7)=3882,a(8)=7028,a(9)=12822,a(10)=23490,a(11)=43364,
 *                 a(12)=80516,a(13)=150510.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper: multiply seq_t by an unsigned int (works for all seq_t types). */
static inline seq_t mul_uint(seq_t x, unsigned int c)
{
    return x * (seq_t)c;
}

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* allocated elements (including index 0) */
static idx_t  cache_len = 0;          /* highest index whose value is known */

/* First 13 terms */
static const seq_t init_values[13] = {
    90, 188, 356, 654, 1190, 2144, 3882,
    7028, 12822, 23490, 43364, 80516, 150510
};

seq_t a(idx_t n)
{
    /* Direct access for small indices (no cache involved). */
    if (n <= 13) {
        return init_values[n - 1];
    }

    /* Ensure cache large enough for indices up to n (index 0 is unused). */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – fall back to returning 0 (undefined behaviour). */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = need;
    }

    /* Initialise cache with the first 13 terms if not yet done. */
    if (cache_len < 13) {
        for (size_t i = 1; i <= 13; ++i) {
            cache[i] = init_values[i - 1];
        }
        cache_len = 13;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        pos += mul_uint(cache[i - 1], 4);
        pos += mul_uint(cache[i - 2], 5);
        pos += mul_uint(cache[i - 4], 6);
        pos += mul_uint(cache[i - 5], 119);
        pos += mul_uint(cache[i - 8], 134);
        pos += mul_uint(cache[i - 9], 130);
        pos += mul_uint(cache[i -12], 20);
        pos += mul_uint(cache[i -13], 8);

        neg += mul_uint(cache[i - 3], 36);
        neg += mul_uint(cache[i - 6], 71);
        neg += mul_uint(cache[i - 7], 180);
        neg += mul_uint(cache[i -10], 90);
        neg += mul_uint(cache[i -11], 48);

        cache[i] = pos - neg;
    }

    cache_len = n;
    return cache[n];
}

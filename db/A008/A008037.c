/*
 * A008037  Coordination sequence T2 for Zeolite Code AST.
 * https://oeis.org/A008037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + 2*a(n-5) + a(n-8) + 2*a(n-10) +
 *            a(n-12) - (2*a(n-2) + a(n-4) + 2*a(n-7) + 3*a(n-9) + a(n-11)),
 *            for n >= 13 Initial values: a(0)=1, a(1)=4, a(2)=12, a(3)=18,
 *            a(4)=28, a(5)=52, a(6)=78, a(7)=88, a(8)=112, a(9)=162,
 *            a(10)=204, a(11)=220, a(12)=254
 * Range:     u8 n <= 12, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values   */
static idx_t cache_len = 0;      /* number of entries stored   */

/* Predefined initial 13 values */
static const seq_t init_vals[13] = {
    (seq_t)1, (seq_t)4, (seq_t)12, (seq_t)18, (seq_t)28,
    (seq_t)52, (seq_t)78, (seq_t)88, (seq_t)112, (seq_t)162,
    (seq_t)204, (seq_t)220, (seq_t)254
};

static void init_cache(void)
{
    /* Allocate space for the first 13 terms (index 0..12) */
    cache = (seq_t*)malloc(sizeof(seq_t) * 13);
    for (int i = 0; i < 13; ++i)
        cache[i] = init_vals[i];
    cache_len = 13;
}

seq_t a(idx_t n)
{
    /* Ensure that the cache is initialized */
    if (cache == NULL)
        init_cache();

    /* Grow the cache if a higher index is needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        /* Compute new terms using the linear recurrence */
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t val = cache[k-1];                    // a(k-1)
            val += cache[k-3] * (seq_t)3;              // +3*a(k-3)
            val += cache[k-5] * (seq_t)2;              // +2*a(k-5)
            val += cache[k-8];                         // +a(k-8)
            val += cache[k-10] * (seq_t)2;             // +2*a(k-10)
            val += cache[k-12];                        // +a(k-12)
            val -= cache[k-2] * (seq_t)2;              // -2*a(k-2)
            val -= cache[k-4];                         // -a(k-4)
            val -= cache[k-7] * (seq_t)2;              // -2*a(k-7)
            val -= cache[k-9] * (seq_t)3;              // -3*a(k-9)
            val -= cache[k-11];                        // -a(k-11)
            cache[k] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}

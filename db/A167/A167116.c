/*
 * A167116  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A167116 by linear recurrence recovered from the rational
 *            generating function. For n = 0..13: use the given initial terms.
 *            For n = 14: a(14) = 13·∑_{k=1}^{13}a(14-k) − 91·a(0) + 1 For n ≥
 *            15: a(n) = 13·∑_{k=1}^{13}a(n-k) − 91·a(n-14)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of cached terms           */
static size_t cache_cap = 0;     /* allocated size of cache array   */
static size_t cache_size = 0;    /* number of terms currently cached*/

seq_t a(idx_t n)
{
    /* Initialise cache with known values for 0 … 13. */
    if (cache_size == 0) {
        /* Allocate a bit more space for future growth. */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */

        /* The first 14 terms (indices 0..13). */
        static const seq_t init[14] = {
            (seq_t)1,            /* a(0)  */
            (seq_t)15,           /* a(1)  */
            (seq_t)210,          /* a(2)  */
            (seq_t)2940,         /* a(3)  */
            (seq_t)41160,        /* a(4)  */
            (seq_t)576240,       /* a(5)  */
            (seq_t)8067360,      /* a(6)  */
            (seq_t)112943040,    /* a(7)  */
            (seq_t)1581202560,   /* a(8)  */
            (seq_t)22136835840,  /* a(9)  */
            (seq_t)309915701760, /* a(10) */
            (seq_t)4338819824640,/* a(11) */
            (seq_t)60743477544960,/* a(12)*/
            (seq_t)850408685629440/* a(13)*/
        };
        for (size_t i = 0; i < 14; ++i) cache[i] = init[i];
        cache_size = 14;
    }

    /* Return cached value if we already have it. */
    if ((size_t)n < cache_size) return cache[(size_t)n];

    /* Ensure enough space for new terms. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n, starting from cache_size. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 14) {                      /* special case for a(14) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 13; ++k) sum += 13 * cache[i - k];
            val = sum - 91 * cache[0] + 1;
        } else {                            /* recurrence for i ≥ 15 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 13; ++k) sum += 13 * cache[i - k];
            val = sum - 91 * cache[i - 14];
        }
        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}

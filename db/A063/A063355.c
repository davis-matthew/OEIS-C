/*
 * A063355  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          82 ).
 * https://oeis.org/A063355
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13. Initial values for 2
 *            ≤ n ≤ 13 are: a(2) = -1, a(3) = 69, a(4) = 140, a(5) = 210, a(6)
 *            = 280, a(7) = 350, a(8) = 420, a(9) = 488, a(10) = 560, a(11) =
 *            628, a(12) = 700, a(13) = 770. Offset: 2
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache to store already computed terms. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;     /* size of allocated cache (number of slots) */
static idx_t max_computed = 1;  /* highest index that has been computed */

/* Initial values for n = 0..13.  Indices 0 and 1 are unused (padding). */
static const seq_t base[14] = {
    0L,   /* 0 not used */
    0L,   /* 1 not used */
   -1L,   /* 2 */
    69L,  /* 3 */
   140L,  /* 4 */
   210L,  /* 5 */
   280L,  /* 6 */
   350L,  /* 7 */
   420L,  /* 8 */
   488L,  /* 9 */
   560L,  /* 10 */
   628L,  /* 11 */
   700L,  /* 12 */
   770L   /* 13 */
};

seq_t a(idx_t n)
{
    /* For indices before the offset, return 0 (not defined). */
    if (n < 2)
        return 0;

    /* If already computed, just return the stored value. */
    if (n <= max_computed)
        return cache[n];

    /* Ensure the cache is large enough to hold index n. */
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /* Compute terms from next uncomputed index up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 13) {
            cache[i] = base[i];
        } else {
            cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
        }
    }
    max_computed = n;
    return cache[n];
}

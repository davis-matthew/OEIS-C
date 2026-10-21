/*
 * A063347  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          74 ).
 * https://oeis.org/A063347
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13, with a(2) = -1, a(3)
 *            = 56, a(4) = 114, a(5) = 171, a(6) = 228, a(7) = 285, a(8) =
 *            342, a(9) = 397, a(10) = 456, a(11) = 511, a(12) = 570, a(13) =
 *            627
 * Range:     i8 n <= 4, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

/* precalculate the first 14 values (indices 0..13) */
static const seq_t initial_vals[14] = {
    0,          /* a(0) not used */
    0,          /* a(1) not used */
   -1,          /* a(2) */
    56,         /* a(3) */
   114,         /* a(4) */
   171,         /* a(5) */
   228,         /* a(6) */
   285,         /* a(7) */
   342,         /* a(8) */
   397,         /* a(9) */
   456,         /* a(10) */
   511,         /* a(11) */
   570,         /* a(12) */
   627          /* a(13) */
};

/* cache for computed values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of entries stored = cache_len */

seq_t a(idx_t n)
{
    /* values for n < 2 are undefined; return 0 to avoid trouble */
    if (n < 2) return 0;

    /* Ensure cache is large enough */
    if (n >= cache_len) {
        /* realloc to size (n+1) */
        size_t new_size = (size_t)(n + 1) * sizeof(seq_t);
        cache = (seq_t *)realloc(cache, new_size);
        if (!cache) return 0;  /* nothing else we can do */

        /* compute values up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 13) {
                cache[i] = initial_vals[i];
            } else {
                cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}

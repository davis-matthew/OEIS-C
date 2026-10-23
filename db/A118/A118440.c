/*
 * A118440  Row sums of triangle A118438.
 * https://oeis.org/A118440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 20, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118440: Row sums of triangle A118438
 * Recurrence:  a(0)=1, a(1)=0, a(2)=4, a(3)=0, a(4)=48, a(5)=160, a(6)=-256, a(7)=0
 * For n>=8:
 *   a(n) = 2*a(n-1) - 4*a(n-2) + 8*a(n-3) - 64*a(n-4)
 *          + 128*a(n-5) - 256*a(n-6) + 512*a(n-7)
 *
 * The recurrence follows from the rational generating function
 *   (1-2x+8x²-16x³+128x⁴-96x⁵+128x⁶-256x⁷) /
 *   ((1-2x)(1+4x²)(1+64x⁴))
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache to store computed values – grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* current capacity (in terms of idx_t) */
    static idx_t max_computed = 0;  /* largest n for which a(n) is stored */

    /* Initialisation: allocate first 8 terms (indices 0..7) */
    if (cache_len == 0) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure – undefined for the problem */

        /* Set known initial values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)48;
        cache[5] = (seq_t)160;
        cache[6] = (seq_t)-256;
        cache[7] = (seq_t)0;

        max_computed = 7;
    }

    /* Grow cache if required to hold up to index n */
    if ((idx_t)(n) >= cache_len) {
        idx_t new_len = cache_len;
        while ((idx_t)(n) >= new_len) new_len <<= 1;  /* double until sufficient */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure – undefined for the problem */
        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 7) {
            /* values 0..7 are already known */
            continue;
        }
        cache[i] =
            (seq_t)2 * cache[i-1]
          - (seq_t)4 * cache[i-2]
          + (seq_t)8 * cache[i-3]
          - (seq_t)64 * cache[i-4]
          + (seq_t)128 * cache[i-5]
          - (seq_t)256 * cache[i-6]
          + (seq_t)512 * cache[i-7];
    }
    max_computed = (n > max_computed) ? n : max_computed;

    return cache[n];
}

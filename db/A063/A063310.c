/*
 * A063310  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          37 ).
 * https://oeis.org/A063310
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13, with initial
 *            values: a(2) = -1, a(3) = 40, a(4) = 96, a(5) = 153, a(6) = 210,
 *            a(7) = 267, a(8) = 324, a(9) = 381, a(10) = 438, a(11) = 495,
 *            a(12) = 552, a(13) = 607
 * Range:     i8 n <= 4, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Ensure n >= 2 */
    if (n < 2) return 0;  /* Undefined for n < 2; caller should avoid it */

    /* Static cache of computed terms (offset 2) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of stored terms; corresponds to indices 2 .. cache_nmax */

    /* Helper to grow cache to at least 'size' elements */
    if (!cache) {
        /* Allocate initial space for terms up to n=13 (12 elements) */
        cache = (seq_t*)malloc(12 * sizeof(seq_t));
        if (!cache) return 0; /* Allocation failure */
        cache[0] = (seq_t)-1;   /* a(2) */
        cache[1] = (seq_t)40;   /* a(3) */
        cache[2] = (seq_t)96;   /* a(4) */
        cache[3] = (seq_t)153;  /* a(5) */
        cache[4] = (seq_t)210;  /* a(6) */
        cache[5] = (seq_t)267;  /* a(7) */
        cache[6] = (seq_t)324;  /* a(8) */
        cache[7] = (seq_t)381;  /* a(9) */
        cache[8] = (seq_t)438;  /* a(10) */
        cache[9] = (seq_t)495;  /* a(11) */
        cache[10] = (seq_t)552; /* a(12) */
        cache[11] = (seq_t)607; /* a(13) */
        cache_size = 12;
    }

    /* Compute terms up to requested n if necessary */
    if (n-2 >= cache_size) {
        idx_t new_size = n-1;  /* number of terms up to n (inclusive) */
        seq_t *temp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!temp) return 0;   /* Allocation failure */
        cache = temp;
        /* Compute new entries */
        for (idx_t idx = cache_size; idx < new_size; ++idx) {
            idx_t cur_n = idx + 2;  /* current n */
            if (cur_n <= 13) {
                /* Should not happen because initial array covers up to 13,
                   but guard just in case */
                /* Values already initialized; skip */
                continue;
            }
            /* Indices for recurrence */
            idx_t i4  = cur_n - 4;  /* n-4 */
            idx_t i6  = cur_n - 6;  /* n-6 */
            idx_t i10 = cur_n - 10; /* n-10 */
            /* Convert to array indices (offset 2) */
            seq_t val = cache[i4 - 2] + cache[i6 - 2] - cache[i10 - 2];
            cache[idx] = val;
        }
        cache_size = new_size;
    }

    return cache[n-2];
}

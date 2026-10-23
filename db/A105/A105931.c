/*
 * A105931  a(1) = 1 then a(n) = a(n-1) - (-1)^ceiling(n/2)*a(floor(n/2)).
 * https://oeis.org/A105931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1 a(n) = a(n-1) - (-1)^ceil(n/2) * a(floor(n/2)) for n >=
 *            2 (sequence values are always -1, 1, or 2)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A105931.
   idx_t and seq_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Static storage for memoization */
    static seq_t *array = NULL;
    static idx_t  size   = 0;   /* allocated size (bytes of indices) */
    static idx_t  computed = 0; /* highest index already computed */

    if (n < 1) {
        return 0; /* undefined for n<1, return 0 as safe default */
    }

    /* Ensure enough space for index n (1‑based) */
    if (size <= (size_t)n) {
        size_t newsize = size ? size * 2 : 16;
        while (newsize <= (size_t)n) newsize <<= 1;   /* grow until large enough */
        seq_t *tmp = (seq_t *)realloc(array, newsize * sizeof(seq_t));
        /* If realloc fails, keep the old array (should not happen in tests) */
        array = tmp;
        size  = newsize;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            array[1] = (seq_t)1;
        } else {
            idx_t  half = i >> 1;                   /* floor(i/2) */
            /* (-1)^ceil(i/2) : 1 if ceil(i/2) even, -1 if odd */
            seq_t sign = (((i + 1) >> 1) & 1) ? (seq_t)-1 : (seq_t)1;
            if (sign == (seq_t)1) {
                array[i] = array[i - 1] - array[half];
            } else {
                array[i] = array[i - 1] + array[half];
            }
        }
    }
    computed = n;
    return array[n];
}

/*
 * A085130  Duplicate of A080194.
 * https://oeis.org/A085130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 7 * b(n) where b(n) is the n‑th 7‑smooth number
 * Range:     i8 n <= 15, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term (1‑based) of A085130. */
seq_t a(idx_t n)
{
    /* dynamic array holding 7‑smooth numbers (starting with 1). */
    static seq_t *smooth = NULL;
    static size_t smooth_len = 0;
    static size_t smooth_cap = 0;

    /* initialise on first call */
    if (smooth_len == 0) {
        smooth_cap = 16;
        smooth = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
        smooth[0] = (seq_t)1;     /* first 7‑smooth number */
        smooth_len = 1;
    }

    /* indices used to generate the next multiples of 2, 3, 5, 7. */
    static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

    /* generate until we have at least n smooth numbers. */
    while (smooth_len < (size_t)n) {
        /* grow storage if necessary */
        if (smooth_len >= smooth_cap) {
            smooth_cap <<= 1;
            smooth = (seq_t *)realloc(smooth, smooth_cap * sizeof(seq_t));
        }

        seq_t next2 = (seq_t)2 * smooth[i2];
        seq_t next3 = (seq_t)3 * smooth[i3];
        seq_t next5 = (seq_t)5 * smooth[i5];
        seq_t next7 = (seq_t)7 * smooth[i7];

        /* find the minimum candidate */
        seq_t next = next2;
        if (next3 < next) next = next3;
        if (next5 < next) next = next5;
        if (next7 < next) next = next7;

        smooth[smooth_len++] = next;

        if (next == next2) ++i2;
        if (next == next3) ++i3;
        if (next == next5) ++i5;
        if (next == next7) ++i7;
    }

    /* a(n) = 7 * smooth[n-1] */
    return (seq_t)7 * smooth[n-1];
}

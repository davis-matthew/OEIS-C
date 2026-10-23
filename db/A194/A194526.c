/*
 * A194526  First coordinate of (5,6)-Lagrange pair for n.
 * https://oeis.org/A194526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 a(1) = -1, a(2) =
 *            -2, a(3) = 3, a(4) = 2, a(5) = 1 , a(6) = 0 , a(7) = -1, a(8) =
 *            -2, a(9) = 3 , a(10)= 2 , a(11)= -1, a(12)= 0
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[12] = {
    -1, -2, 3, 2, 1, 0, -1, -2, 3, 2, -1, 0
};

seq_t a(idx_t n)
{
    /* 0 is undefined for this sequence – guard just for safety */
    if (n == 0) return 0;

    if (n <= 12) {
        /* the first 12 terms are known explicitly */
        return init_vals[n - 1];
    }

    /* For n > 12 compute the recurrence on the fly.
       Use dynamic allocation so the function works for any n
       without pre‑allocating large static arrays. */
    size_t len = (size_t)n;                /* number of elements needed */
    seq_t *seq = (seq_t *)malloc(len * sizeof(seq_t));
    if (!seq) return 0;                    /* out of memory – unlikely for n ≤ 10000 */

    /* copy the initial 12 values */
    for (size_t i = 0; i < 12; ++i) {
        seq[i] = init_vals[i];
    }

    /* iterate from position 13 to n, using 0‑based indexing */
    for (size_t i = 12; i < len; ++i) {
        seq[i] = seq[i - 1] + seq[i - 11] - seq[i - 12];
    }

    seq_t result = seq[len - 1];
    free(seq);
    return result;
}

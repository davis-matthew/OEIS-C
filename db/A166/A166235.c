/*
 * A166235  Triangle T(n,m) of the coefficients [x^m] of the polynomial
 *          ((x-1)*(x+2)*(x+1))^n, 0<=m<=3n.
 * https://oeis.org/A166235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 26, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166235: flattened triangle of [(x‑1)(x+2)(x+1)]^n coefficients */
#include <stddef.h>
#include <stdlib.h>

static const seq_t coeffs[4] = { (seq_t)-2, (seq_t)-1, (seq_t)2, (seq_t)1 };

seq_t a(idx_t n)
{
    /* find the row r such that the first r+1 rows contain > n terms */
    idx_t r = 0;
    size_t cum = 0;
    while (1) {
        size_t len = (size_t)(3 * r + 1);
        if (cum + len > (size_t)n)
            break;
        cum += len;
        r++;
    }
    /* m is the index inside row r */
    idx_t m = n - (idx_t)cum;

    /* row 0 is a single 1 */
    if (r == 0)
        return (seq_t)1;

    /* iterate rows until we reach row r */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    prev[0] = (seq_t)1;
    size_t prev_len = 1;

    for (idx_t i = 0; i < r; i++) {
        size_t new_len = 3 * (i + 1) + 1;
        seq_t *next = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (size_t j = 0; j < new_len; j++)
            next[j] = (seq_t)0;

        for (size_t j = 0; j < prev_len; j++) {
            for (int k = 0; k < 4; k++) {
                next[j + k] += prev[j] * coeffs[k];
            }
        }

        if (i + 1 == r) {
            seq_t result = next[m];
            free(next);
            free(prev);
            return result;
        }

        free(prev);
        prev = next;
        prev_len = new_len;
    }

    /* should never reach here */
    free(prev);
    return (seq_t)0;
}

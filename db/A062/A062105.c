/*
 * A062105  Square array read by antidiagonals: number of ways a pawn-like
 *          piece (with the initial 2-step move forbidden and starting from
 *          any square on the back rank) can end at various squares on an
 *          infinite chessboard.
 * https://oeis.org/A062105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the value at antidiagonal n of the array whose * rows
 *            satisfy the recurrence f[i][j] = f[i-1][j-1] + f[i-1][j] +
 *            f[i-1][j+1], * with f[0][j] = 1. The array is read by
 *            antidiagonals: * a(0)=f[0][0], a(1)=f[0][1], a(2)=f[1][0],
 * Range:     u8 n <= 26, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Locate the antidiagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) {
        ++d;
    }
    idx_t lower = d * (d + 1) / 2;
    idx_t row = n - lower;          /* row index in the diagonal d */
    idx_t col = d - row;            /* column index in the diagonal d */

    /* Row 0 consists entirely of 1's */
    if (row == 0) {
        return (seq_t)1;
    }

    /* Allocate two working arrays of size d+2 (extra space to avoid bounds checks) */
    size_t sz = (size_t)d + 2;
    seq_t *prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;   /* sentinel for allocation failure */
    }

    /* Base row (row 0): all 1's up to column d */
    for (idx_t i = 0; i <= d; ++i) {
        prev[i] = (seq_t)1;
    }

    /* Compute rows 1 .. row */
    for (idx_t r = 1; r <= row; ++r) {
        idx_t cmax = d - r;   /* largest column index needed at this row */

        for (idx_t c = 0; c <= cmax; ++c) {
            seq_t val = prev[c];           /* value from directly below */
            if (c + 1 <= cmax + 1) {       /* from diagonal right in prev row */
                val += prev[c + 1];
            }
            if (c > 0) {                    /* from diagonal left in prev row */
                val += prev[c - 1];
            }
            curr[c] = val;
        }

        /* Zero unused tail of curr for safety (optional) */
        for (idx_t c = cmax + 1; c < sz; ++c) {
            curr[c] = (seq_t)0;
        }

        /* Swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[col];
    free(prev);
    free(curr);
    return result;
}

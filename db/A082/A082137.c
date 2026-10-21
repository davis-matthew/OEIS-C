/*
 * A082137  Square array of transforms of binomial coefficients, read by
 *          antidiagonals.
 * https://oeis.org/A082137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 97, u32 n <= 288, u64 n <= 973,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A082137
   Square array of transforms of binomial coefficients, read by antidiagonals.
   Formula (recurrent definition of the square array):
     T(0,0) = 1,
     T(1,0) = T(1,1) = 1,
     T(n,k) = 2*T(n-1,k) + 2*T(n-1,k-1) - T(n-2,k) - 2*T(n-2,k-1)
             for n >= 2, 0 <= k <= n, with T(n,k)=0 when k<0 or k>n.
   a(n) is the antidiagonal reading of the triangle T, i.e. a(n)=T(i,j)
   where i and j satisfy n = i*(i+1)/2 + j (0‑based indices).  */

/* ---------- */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row i of the antidiagonal in which index n lies.
       For row r, the number of entries up to and including that row is
          (r+1)(r+2)/2.  We find the smallest r with this value > n,
       then subtract one.  */
    idx_t row = 0;
    while ((row * (row + 1)) / 2 <= n)
        ++row;
    --row;                    /* row is now the correct row index */

    /* Find column (k) within that row */
    idx_t start = (row * (row + 1)) / 2;   /* first index of this row */
    idx_t col   = n - start;                /* column 0‑based */

    /* Two previous rows are kept to apply the recurrence. */
    seq_t *prevprev = NULL;   /* row n-2 */
    seq_t *prev     = NULL;   /* row n-1 */
    seq_t result    = 0;

    for (idx_t r = 0; r <= row; ++r) {
        /* Allocate memory for current row of size r+1.  */
        seq_t *cur = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
        if (!cur)          /* allocation failure (should not happen) */
            return 0;

        for (idx_t k = 0; k <= r; ++k) {
            seq_t val = 0;
            if (r == 0 && k == 0)
                val = 1;                    /* T(0,0) */
            else if (r == 1 && (k == 0 || k == 1))
                val = 1;                    /* T(1,0) = T(1,1) = 1 */
            else {
                /* Retrieve the needed elements of the two previous rows.
                   Default to 0 when the indices are out of bounds. */
                seq_t a1 = (prev && k <= r - 1 ? prev[k] : 0);
                seq_t b1 = (prev && k > 0         ? prev[k - 1] : 0);
                seq_t c1 = (prevprev && k <= r - 2 ? prevprev[k] : 0);
                seq_t d1 = (prevprev && k > 0 && k - 1 <= r - 2
                            ? prevprev[k - 1] : 0);

                val = 2 * a1 + 2 * b1 - c1 - 2 * d1;
            }
            cur[k] = val;
            if (r == row && k == col)
                result = val;                /* found the wanted entry */
        }

        /* Shift rows for next iteration.  Free the row that is no longer needed. */
        free(prevprev);
        prevprev = prev;
        prev     = cur;
    }

    /* Clean up the remaining memory. */
    free(prevprev);
    free(prev);

    return result;
}

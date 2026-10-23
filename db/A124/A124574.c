/*
 * A124574  Triangle read by rows: row n is the first row of the matrix
 *          M[n]^(n-1), where M[n] is the n X n tridiagonal matrix with main
 *          diagonal (3,4,4,...) and super- and subdiagonals (1,1,1,...).
 * https://oeis.org/A124574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of the triangle read by rows. * The
 *            triangle T(i,j) is defined by * * T(0,0) = 1 , T(i,j) = 0 if j <
 *            0 or j > i * T(i,0) = 3*T(i‑1,0) + T(i‑1,1) * T(i,j) =
 *            T(i‑1,j‑1) + 4*T(i‑1,j) + T(i‑1,j+1) for j ≥ 1 * * a(1)=T(0,0),
 *            a(2)=T(1,0), a(3)=T(1,1), a(4)=T(2,0), … (offset 1) * *...
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 120, u64 n <= 379,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence has offset 1 – n=0 is never requested */
        return 0;

    /* Find the row index r (0‑based) that contains the n‑th term.
     * Total terms up to row r is (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n)
        ++r;

    /* Number of terms before row r. */
    idx_t prevCount = r * (r + 1) / 2;
    idx_t k = n - prevCount - 1;   /* position inside row r (0‑based) */

    /* Allocate space for two rows, each of length r+1 (maximum needed). */
    seq_t *prev = (seq_t *)calloc(r + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(r + 1, sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return 0;   /* allocation failure fallback */
    }

    /* Row 0 : T(0,0)=1 */
    prev[0] = 1;

    /* Build rows 1 … r.  After the loop, 'prev' contains row r. */
    for (idx_t row = 1; row <= r; ++row) {
        idx_t lenPrev = row;         /* previous row length */
        idx_t lenCurr = row + 1;     /* current row length */

        for (idx_t j = 0; j < lenCurr; ++j) {
            seq_t left  = (j > 0) ? prev[j - 1] : 0;
            seq_t mid   = (j < lenPrev) ? prev[j] : 0;
            seq_t right = (j + 1 < lenPrev) ? prev[j + 1] : 0;

            if (j == 0) {
                curr[0] = 3 * mid + right;
            } else if (j == lenCurr - 1) {
                curr[j] = left;          /* T(i,i)=T(i‑1,i‑1) */
            } else {
                curr[j] = left + 4 * mid + right;
            }
        }

        /* Swap prev and curr for the next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];

    free(prev);
    free(curr);
    return result;
}

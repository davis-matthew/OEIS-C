/*
 * A209127  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209126; see the Formula section.
 * https://oeis.org/A209127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A209127 – Triangle of coefficients of v(n,x)
 *
 *  Formula:
 *      T(1,0) = 1
 *      T(2,0) = 0 ,  T(2,1) = 2
 *      T(n,k) = T(n‑1,k) + T(n‑1,k‑1) + T(n‑2,k‑2)   (n ≥ 3, 0 ≤ k < n)
 *      T(n,k) = 0 if k < 0 or k ≥ n
 *
 *  a(n) is obtained by flattening the rows of the triangle:
 *      a(1) = T(1,0)
 *      a(2) = T(2,0), a(3) = T(2,1)
 *      a(4) = T(3,0), a(5) = T(3,1), a(6) = T(3,2)
 *      ...
 *  i.e. for each n find the unique row r with r(r+1)/2 ≥ n,
 *  let S(r‑1) = (r‑1)r/2, then k = n – S(r‑1) – 1 and a(n) = T(r,k).
 *
 *  The implementation builds the required row on the fly in O(r²)
 *  time (r ≈ √(2n)).  For n ≤ 10 000 the maximum r is 141,
 *  so the routine is easily fast enough for any list of queries.
 */

#include <stddef.h>   /* for idx_t if needed */

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Special case: first row */
    if (r == 1) {
        return 1;
    }

    /*----------------------------------------------------------------------*/
    /*  We store at most MAX_R elements.  For n ≤ 10 000, r ≤ 141, so 200
     *  gives generous safety.
     */
    /*----------------------------------------------------------------------*/
    const idx_t MAX_R = 200;   /* should be ≥ 141 */

    /* Rows are computed iteratively; previous two rows are preserved. */
    seq_t row_prev2[MAX_R];   /* corresponds to T(n‑2,·) */
    seq_t row_prev1[MAX_R];   /* corresponds to T(n‑1,·) */
    seq_t row_cur  [MAX_R];   /* corresponding to T(n,·)   */

    /* Initialise row 1 */
    row_prev2[0] = 1;          /* T(1,0) */

    /* Initialise row 2 */
    row_prev1[0] = 0;          /* T(2,0) */
    row_prev1[1] = 2;          /* T(2,1) */

    /* If we only need the second row, answer directly. */
    if (r == 2) {
        idx_t S_prev = 1;      /* S(1) = 1*2/2 = 1 */
        idx_t k = n - S_prev - 1;
        return (k == 0) ? row_prev1[0] : row_prev1[1];
    }

    /* Build subsequent rows up to row r. */
    for (idx_t i = 3; i <= r; ++i) {
        /* compute T(i, k) for k = 0 .. i‑1  */
        for (idx_t k = 0; k < i; ++k) {
            seq_t val = 0;

            /* T(i‑1, k) contributes if k < i‑1 */
            if (k < i - 1) {
                val += row_prev1[k];
            }

            /* T(i‑1, k‑1) contributes if k ≥ 1 */
            if (k >= 1) {
                val += row_prev1[k - 1];
            }

            /* T(i‑2, k‑2) contributes if k ≥ 2 */
            if (k >= 2) {
                val += row_prev2[k - 2];
            }

            row_cur[k] = val;
        }

        /* Shift rows for next iteration: row_prev2 ← row_prev1,
         * row_prev1 ← row_cur   */
        for (idx_t k = 0; k < i - 1; ++k) {
            row_prev2[k] = row_prev1[k];
        }
        for (idx_t k = 0; k < i; ++k) {
            row_prev1[k] = row_cur[k];
        }
    }

    /* Now row_prev1 contains T(r,·).  Compute the column index k. */
    idx_t S_prev = r * (r - 1) / 2;   /* S(r‑1) */
    idx_t k = n - S_prev - 1;        /* 0‑based column within row r */

    return row_prev1[k];
}

/*
 * A183632  T(n,k) = number of (n+1) X (k+1) 0..2 arrays with every 2 X 2
 *          subblock summing to 4.
 * https://oeis.org/A183632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 190, u64 n <= 219,
 *            u128 n <= 219, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS entry A183632:  T(n,k) is the number of (n+1)×(k+1) 0..2 arrays
 *  whose every 2×2 subblock sums to 4.  
 *  The triangle T(n,k) (for n,k ≥ 1) satisfies the linear recurrences
 *      T(n,k) = 6·T(n‑1,k) – 11·T(n‑2,k) + 6·T(n‑3,k)   (n ≥ 4)
 *  and the same formula with respect to the second index k (columns).
 *  The b‑file lists the table entries in antidiagonal order; a(n) is the
 *  term that appears at the n‑th position of that ordering.  The code
 *  constructs the triangle up to the required size and returns the
 *  appropriate entry.  Initial values for the 3×3 lower left corner are
 *      T(1,1)=19, T(1,2)=45,  T(1,3)=115
 *      T(2,1)=45, T(2,2)=87,  T(2,3)=189
 *      T(3,1)=115, T(3,2)=189, T(3,3)=355                     */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used; offset is 1 */

    /* find the diagonal sum s such that (s-1)*s/2 < n <= s*(s-1)/2  */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n) ++s;

    /* coordinates of the n‑th entry in antidiagonal order */
    idx_t prev = (s - 2) * (s - 1) / 2;          /* elements before this diagonal */
    idx_t offset = n - prev - 1;                /* 0‑based index within the diagonal */
    idx_t i = offset + 1;                       /* row (1‑based) */
    idx_t j = s - offset - 1;                   /* column (1‑based) */

    /* maximum index needed in either dimension */
    idx_t max_dim = s - 1;

    /* allocate a square table (max_dim+1)×(max_dim+1); index 0 row/column unused */
    size_t size = (size_t)(max_dim + 1) * (size_t)(max_dim + 1);
    seq_t *tbl = (seq_t *)calloc(size, sizeof(seq_t));
    if (!tbl) return (seq_t)0;                  /* allocation failure (unlikely) */

    /* linear index macro (row-major) */
    #define IDX(x, y) ((((size_t)(x)) * (max_dim + 1)) + (size_t)(y))

    /* ------------------------------------------------------------
       Initialise the 3×3 lower left corner from the OEIS data
       ------------------------------------------------------------ */
    if (1 <= max_dim)                          tbl[IDX(1, 1)] = (seq_t)19;
    if (1 <= max_dim && 2 <= max_dim)          tbl[IDX(1, 2)] = (seq_t)45;
    if (1 <= max_dim && 3 <= max_dim)          tbl[IDX(1, 3)] = (seq_t)115;
    if (2 <= max_dim && 1 <= max_dim)          tbl[IDX(2, 1)] = (seq_t)45;
    if (2 <= max_dim && 2 <= max_dim)          tbl[IDX(2, 2)] = (seq_t)87;
    if (2 <= max_dim && 3 <= max_dim)          tbl[IDX(2, 3)] = (seq_t)189;
    if (3 <= max_dim && 1 <= max_dim)          tbl[IDX(3, 1)] = (seq_t)115;
    if (3 <= max_dim && 2 <= max_dim)          tbl[IDX(3, 2)] = (seq_t)189;
    if (3 <= max_dim && 3 <= max_dim)          tbl[IDX(3, 3)] = (seq_t)355;

    /* ------------------------------------------------------------
       Fill the remaining entries by iterating over diagonals.
       ------------------------------------------------------------ */
    for (idx_t sum = 2; sum <= s; ++sum) {
        for (idx_t row = 1; row < sum; ++row) {
            idx_t col = sum - row;
            if (row > max_dim || col > max_dim) continue;

            /* Skip the base entries already filled */
            if (row <= 3 && col <= 3) continue;

            if (row >= 4) {
                /* use the row recurrence */
                seq_t v1 = tbl[IDX(row - 1, col)];
                seq_t v2 = tbl[IDX(row - 2, col)];
                seq_t v3 = tbl[IDX(row - 3, col)];
                tbl[IDX(row, col)] = (seq_t)6 * v1 - (seq_t)11 * v2 + (seq_t)6 * v3;
            } else {
                /* use the column recurrence (row <= 3, col >= 4) */
                seq_t v1 = tbl[IDX(row, col - 1)];
                seq_t v2 = tbl[IDX(row, col - 2)];
                seq_t v3 = tbl[IDX(row, col - 3)];
                tbl[IDX(row, col)] = (seq_t)6 * v1 - (seq_t)11 * v2 + (seq_t)6 * v3;
            }
        }
    }

    seq_t result = tbl[IDX(i, j)];
    free(tbl);
    return result;
}

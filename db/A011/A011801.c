/*
 * A011801  Triangle read by rows, the inverse Bell transform of
 *          n!*binomial(4,n) (without column 0).
 * https://oeis.org/A011801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the unique integer with * r(r-1)/2 < n
 *            ≤ r(r+1)/2 and k = n - r(r-1)/2. * Recurrence for the triangle
 *            T: * T(1,1) = 1 * T(n+1,m) = (5*n - m) * T(n,m) + T(n,m-1) for n
 *            ≥ m ≥ 1, * with T(n,0) = 0 and T(n,m) = 0 for n < m. * a(n) is
 *            the nth element of the triangle read by rows
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 105, u128 n <= 300,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find row r containing the nth element */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* position k in row r (1‑indexed) */
    idx_t prev_terms = r * (r - 1) / 2;
    idx_t k = n - prev_terms;

    /* allocate two rows of size r+2 (indices 0..r+1) */
    seq_t *prev = (seq_t*)calloc((size_t)(r + 2), sizeof(seq_t));
    seq_t *curr = (seq_t*)calloc((size_t)(r + 2), sizeof(seq_t));

    /* base row T(1,1) = 1 */
    prev[1] = 1;

    /* build rows up to row r */
    for (idx_t row = 1; row < r; ++row) {
        /* compute next row (row+1) into curr[1..row+1] */
        for (idx_t m = 1; m <= row + 1; ++m) {
            seq_t t1 = (m <= row) ? prev[m] : 0;                     /* T(row,m) */
            seq_t t2 = (m >= 2 && m - 1 <= row) ? prev[m - 1] : 0;   /* T(row,m-1) */
            seq_t factor = (seq_t)(5 * row - m);
            curr[m] = factor * t1 + t2;
        }
        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* result is the k-th element of the final row (prev) */
    seq_t result = prev[k];

    /* free temporary storage */
    free(prev);
    free(curr);

    return result;
}

/*
 * A210039  Array of coefficients of polynomials u(n,x) jointly generated with
 *          A210040; see the Formula section.
 * https://oeis.org/A210039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r+1, 2k+2) where r is the row containing the
 *            n‑th element in the flattened array of coefficients of the
 *            polynomials u(r,x). The j‑th coefficient in row r (0‑based)
 *            equals C(r+1, 2j+2). The row lengths follow 1,1,2,2,3,3,…, i.e.
 *            len(row) = (row+1)/2. The function converts the globa...
 * Range:     u8 n <= 13, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th term. */
    idx_t r = 0;          /* current row number (1‑based)          */
    idx_t cumulative = 0; /* total number of terms up to the current row */

    while (1) {
        r++;
        idx_t len = (r + 1) / 2;      /* length of this row */
        cumulative += len;
        if (cumulative >= n) break;   /* n lies in this row          */
    }

    /* Position of the term within the row (0‑based). */
    idx_t len  = (r + 1) / 2;
    idx_t prev = cumulative - len;   /* sum of terms before this row   */
    idx_t pos  = n - prev - 1;       /* index inside the row           */

    /* Compute the coefficient: C(r+1, 2*pos+2). */
    return binomial(r + 1, (idx_t)(2 * pos + 2));
}

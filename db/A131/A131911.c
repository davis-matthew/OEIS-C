/*
 * A131911  2*A131821 - A128174.
 * https://oeis.org/A131911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (r,c) entry of the lower‑triangular triangle defined
 *            for r≥1, 1≤c≤r by T(r,1) = 2r if r is even = 2r-1 if r is odd
 *            T(r,r) = 2r-1 for 2≤c≤r-1 if r is even => T(r,c)= (c even)?1:2
 *            if r is odd => T(r,c)= (c even)?2:1 A(n) is obtained by
 *            flattening the triangle row by row starting with a(...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Find the row r such that the n‑th element lies in this row. */
    idx_t r = 1;          /* current row */
    idx_t cum = 1;        /* cumulative number of elements up to row r */
    while (cum < n) {
        ++r;
        cum += r;         /* cum now equals r(r+1)/2 */
    }
    /* Starting index of this row (1‑based). */
    idx_t row_start = cum - r + 1;
    /* Column index (1‑based) within the row. */
    idx_t c = n - row_start + 1;

    /* Compute the triangle entry T(r,c). */
    idx_t val;
    if (c == 1) {
        /* First column: numbers congruent to 0 or 1 modulo 4. */
        val = (r & 1) ? (2 * r - 1) : (2 * r);
    } else if (c == r) {
        /* Diagonal entry: 2r-1. */
        val = 2 * r - 1;
    } else {
        if (r & 1) {           /* r odd */
            val = (c & 1) ? 1 : 2; /* odd column →1, even →2 */
        } else {                /* r even */
            val = (c & 1) ? 2 : 1; /* odd column →2, even →1 */
        }
    }

    return (seq_t)val;
}

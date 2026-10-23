/*
 * A135151  A002260 + A128174 - I, I = Identity matrix.
 * https://oeis.org/A135151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value at row r and column c of * A002260 + A128174 - I *
 *            where n is the linear (row‑major) index of the triangle. * Let r
 *            be the smallest integer with r(r+1)/2 ≥ n, * c = n - r(r-1)/2.
 *            Then * a(n) = c + ((c + r - 1) & 1) - (c == r ? 1 : 0) * *
 *            A002260: lower‑triangular with entries j (row...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool, but not strictly required */

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t prevTri = 0;
    while (true) {
        idx_t currTri = prevTri + r;        /* r(r+1)/2 for current r */
        if (n <= currTri) break;
        prevTri = currTri;
        ++r;
    }

    /* Column c within that row */
    idx_t c = n - prevTri;   /* 1‑based */

    /* Compute the value using the formula */
    idx_t parity = (c + r - 1) & 1;      /* ((c + r - 1) mod 2) */
    idx_t delta  = (c == r) ? 1 : 0;      /* subtract 1 on the diagonal */

    seq_t value = (seq_t)c + (seq_t)parity - (seq_t)delta;
    return value;
}

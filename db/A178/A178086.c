/*
 * A178086  Triangle T(n,m) = - phi(n+1) + phi(m+1) + phi(n-m+1), 0<=m<=n,
 *          where phi = A000010 is Euler's totient.
 * https://oeis.org/A178086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,c) where T(r,c) = -phi(r+1) + phi(c+1) + phi(r-c+1)
 *            and (r,c) is the unique pair with 0 <= c <= r and r*(r+1)/2 <= n
 *            < (r+1)*(r+2)/2. This flattens the triangle T row‑by‑row
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"   /* provides static seq_t phi(idx_t m) */

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    idx_t c = n - r * (r + 1) / 2;      /* column within the row, 0 <= c <= r */

    /* Compute T(r,c) = -phi(r+1) + phi(c+1) + phi(r-c+1) */
    seq_t term = -phi(r + 1) + phi(c + 1) + phi(r - c + 1);
    return term;
}

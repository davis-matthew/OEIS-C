/*
 * A213844  Rectangular array: (row n) = b**c, where b(h) = 2*h-1, c(h) =
 *          4*n-5+4*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row,col) where row and col are obtained from the
 *            anti‑diagonal enumeration of the array T. For n≥1 let s be the
 *            smallest integer with (s-1)*s/2 ≥ n. Then t = n - (s-2)*(s-1)/2
 *            row = t col = s - t and T(r,c) = (4*c^3 + 12*r*c^2 - 9*c^2 +
 *            2*c) / 3
 * Range:     u8 n <= 6, u16 n <= 325, u32 n <= 325, u64 n <= 325,
 *            u128 n <= 325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t s = 2;
    /* Find minimal s such that (s-1)*s/2 >= n.  Use 128‑bit arithmetic to
       avoid overflow for very large n. */
    while (((__int128)(s - 1) * s / 2) < n) {
        ++s;
    }
    idx_t prev = (idx_t)(((__int128)(s - 2) * (s - 1)) / 2);
    idx_t t = n - prev;          /* position inside the diagonal */
    idx_t row = t;               /* row index   (≥1) */
    idx_t col = s - t;           /* column index(≥1) */

    seq_t k = (seq_t)col;
    seq_t r = (seq_t)row;

    seq_t term = (seq_t)4 * k * k * k
               + (seq_t)12 * k * k * r
               - (seq_t)9  * k * k
               + (seq_t)2  * k;

    return term / 3;
}

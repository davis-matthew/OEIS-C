/*
 * A001713  Generalized Stirling numbers, [n+6,6]_3.
 * https://oeis.org/A001713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^3] Product_{r=0..n+2} (x + 3 + r), computed by
 *            multiplying the factors in one at a time and keeping only the
 *            coefficients of x^0..x^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t c0 = 1, c1 = 0, c2 = 0, c3 = 0;
    idx_t r;
    for (r = 0; r < n + 3; r++) {
        seq_t m = (seq_t)r + 3;
        c3 = c3 * m + c2;
        c2 = c2 * m + c1;
        c1 = c1 * m + c0;
        c0 = c0 * m;
    }
    return c3;
}

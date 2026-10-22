/*
 * A005313  Maximal sum of inverse squares of the singular values of
 *          triangular anti-Hadamard matrices of order n.
 * https://oeis.org/A005313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 3*a(n-3) + 4*a(n-4) - a(n-5), a(1)
 *            = 1, a(2) = 3, a(3) = 6, a(4) = 13, a(5) = 29 (from the g.f.
 *            x(1-x-3x^2+x^3)/((1-3x+x^2)(1+x)(1-x)^2))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t a1, a2, a3, a4, a5, t;
    idx_t i;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)13;
    if (n == 5) return (seq_t)29;
    a1 = 29; a2 = 13; a3 = 6; a4 = 3; a5 = 1;
    for (i = 6; i <= n; i++) {
        t = (a1 * 4 + a4 * 4) - (a2 * 3 + a3 * 3 + a5);
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = t;
    }
    return a1;
}

/*
 * A007016  Number of permutations of length n with 1 fixed and 1 reflected
 *          point.
 * https://oeis.org/A007016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*m) = m*(x(2*m) - (2*m-3)*x(2*m-1)), a(2*m+1) =
 *            (2*m+1)*x(2*m) + 3*m*x(2*m-1) - 2*m*(m-1)*x(2*m-2), where x(n) =
 *            A003471(n), with a(0)=0, a(1)=1, a(2)=0, a(3)=0. x is computed
 *            via I(m) = x(2m) = Integral_{0..inf} e^-u (u^2-4u+2)^m du and
 *            J(m) = Integral e^-u u (u^2-4u+2)^m du, x(2m+1) = J(m) -...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 4) return (seq_t)(n == 1 ? 1 : 0);
    idx_t m = n / 2;
    seq_t I = 1, J = 1, p2 = 1;
    seq_t Ip = 0, Jp = 0;
    idx_t k;
    for (k = 1; k <= m; k++) {
        seq_t kk = (seq_t)k;
        p2 *= 2;
        seq_t In = p2 + kk * 2 * J - kk * 4 * I;
        seq_t Jn = (kk * 2 + 1) * In + kk * 4 * J - kk * 4 * I;
        Ip = I;
        Jp = J;
        I = In;
        J = Jn;
    }
    /* I = x(2m), Jp - Ip = x(2m-1), Ip = x(2m-2) */
    seq_t xe = I;
    seq_t xo = Jp - Ip;
    seq_t xpe = Ip;
    seq_t sm = (seq_t)m;
    if (n % 2 == 0) {
        return sm * (xe - (sm * 2 - 3) * xo);
    } else {
        return (sm * 2 + 1) * xe + sm * 3 * xo - sm * 2 * (sm - 1) * xpe;
    }
}

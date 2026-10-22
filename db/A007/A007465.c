/*
 * A007465  Exponential-convolution of triangular numbers with themselves.
 * https://oeis.org/A007465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + n*2^(n+1) + 5*n*(n-1)*2^(n-2) +
 *            n*(n-1)*(n-2)*2^(n-2) + 6*binomial(n,4)*2^(n-4), from E.g.f.
 *            (1/4)*exp(2*x)*(2 + 4*x + x^2)^2 = exp(2*x)*(1 + 4*x + 5*x^2 +
 *            2*x^3 + x^4/4)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   p2_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/p2_seq.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t r = p2_seq(n);
    r += N * p2_seq(n + 1);
    if (n >= 2) {
        seq_t f2 = N * (N - 1);
        r += 5 * f2 * p2_seq(n - 2);
        if (n >= 3) {
            r += f2 * (N - 2) * p2_seq(n - 2);
            if (n >= 4) {
                seq_t c = N * (N - 1) / 2;
                c = c * (N - 2) / 3;
                c = c * (N - 3) / 4;
                r += 6 * c * p2_seq(n - 4);
            }
        }
    }
    return r;
}

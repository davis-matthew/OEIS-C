/*
 * A001236  Differences of reciprocals of unity.
 * https://oeis.org/A001236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)!^3 * Sum_{i=1..n+1} Sum_{j=1..i} Sum_{k=1..j}
 *            1/(i*j*k), computed with m = n+1 and A_k(m) = m!^k *
 *            h_k(1,1/2,...,1/m): A_k(m) = m^k*A_k(m-1) + (m-1)!*A_{k-1}(m),
 *            A_0 = 1, A_k(1) = 1 (fixed-width build); arbitrary-precision
 *            build: a = (U^3 + 3*U*V + 2*W)/6 with U = m!*H(m,1), V = m!^2...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   a001236_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001236_fixed_width.h"

seq_t a(idx_t n) {
    idx_t m = n + 1, i;
    if (a001236_fixed_width()) {
        /* only + and *: exact modulo 2^w, hence exact whenever the result fits */
        seq_t A1 = 1, A2 = 1, A3 = 1, f = 1; /* f = (i-1)! */
        for (i = 2; i <= m; i++) {
            seq_t si = (seq_t)i;
            f = f * (seq_t)(i - 1);
            A1 = si * A1 + f;
            A2 = si * si * A2 + f * A1;
            A3 = si * si * si * A3 + f * A2;
        }
        return A3;
    } else {
        seq_t U = 0, V = 0, W = 0;
        seq_t F = 1, F2 = 1, F3 = 1; /* (i-1)!, (i-1)!^2, (i-1)!^3 */
        for (i = 1; i <= m; i++) {
            seq_t s1 = (seq_t)i;
            seq_t s2 = s1 * s1;
            seq_t s3 = s2 * s1;
            U = U * s1 + F;
            V = V * s2 + F2;
            W = W * s3 + F3;
            F *= s1;
            F2 *= s2;
            F3 *= s3;
        }
        return (U * U * U + 3 * U * V + 2 * W) / 6;
    }
}

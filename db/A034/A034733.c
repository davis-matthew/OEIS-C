/*
 * A034733  Dirichlet convolution of b_n=2^(n-1) with itself.
 * https://oeis.org/A034733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} 2^{d + n/d - 2} (Dirichlet convolution of 2^{n-1}
 *            with itself)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour for n < 1 is irrelevant */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            /* exponent = d + q - 2;  keep within idx_t range for expected n */
            seq_t val = pow2(d + q - 2);
            sum += val;
            if (d != q) sum += val;   /* include the paired divisor */
        }
    }
    return sum;
}

/*
 * A107995  Chebyshev polynomial of the second kind U[n,x] evaluated at x=n+2.
 * https://oeis.org/A107995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2*n+2)^k * binomial(n+1+k, 2*k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)(2 * n + 2);
    seq_t sum = 0;
    seq_t pow = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n + 1 + k, 2 * k + 1);
        sum += pow * bin;
        pow *= base;
    }
    return sum;
}

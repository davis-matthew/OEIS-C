/*
 * A038744  a(n) = T(2n,n), array T as in A038738.
 * https://oeis.org/A038744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n + k + 1, n - k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t N = 3*n + k + 1;
        idx_t K = n - k;
        sum += binomial(N, K);
    }
    return sum;
}

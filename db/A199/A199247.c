/*
 * A199247  G.f. satisfies: A(x) = (1 + x*A(x))*(1 + x^2*A(x)^3 + x^3*A(x)^4).
 * https://oeis.org/A199247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n+k,k) * C(n+2k+1,n-2k) / (n+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t term1 = binom(n + k, k);
        seq_t term2 = binom(n + 2 * k + 1, n - 2 * k);
        sum += term1 * term2;
    }
    return sum / (seq_t)(n + 1);
}

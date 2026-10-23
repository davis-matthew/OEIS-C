/*
 * A192441  Coefficient of x^(2*n) in the expansion of (1 + x^3 + x^4)^n.
 * https://oeis.org/A192441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=ceil(n/2)}^{floor(2*n/3)}
 *            binomial(n,k)*binomial(k,2*n-3*k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t start = (n + 1) / 2;       /* ceil(n/2)   */
    idx_t end   = (2 * n) / 3;       /* floor(2n/3) */
    seq_t sum = 0;
    for (idx_t k = start; k <= end; ++k) {
        seq_t term = binomial(n, k);
        idx_t m = 2 * n - 3 * k;
        term *= binomial(k, m);
        sum += term;
    }
    return sum;
}

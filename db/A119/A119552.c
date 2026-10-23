/*
 * A119552  Binomial(binomial(2*n,n)*n,n).
 * https://oeis.org/A119552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(binomial(2*n, n)*n, n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n == 0)
        return 1;
    seq_t inner = binomial(2 * n, n);          /* C(2n,n) */
    seq_t M = inner * (seq_t)n;                /* M = C(2n,n) * n */
    for (idx_t i = 1; i <= n; ++i) {
        result *= (M - (i - 1));
        result /= i;
    }
    return result;
}

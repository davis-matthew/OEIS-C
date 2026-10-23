/*
 * A036068  Expansion of (-1+1/(1-3*x)^3)/(9*x).
 * https://oeis.org/A036068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^(n-1) * binomial(n+3, 2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t bin = binomial(n + 3, 2);
    seq_t pow = pow3(n - 1);
    return pow * bin;
}

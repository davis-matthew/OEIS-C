/*
 * A036083  Expansion of (-1+1/(1-5*x)^5)/(25*x); related to A036071.
 * https://oeis.org/A036083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^(n-1) * binomial(n+5, 4) (for n ≥ 1) * a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) */

    /* compute binomial(n+5, 4) */
    seq_t bin = binomial(n + (idx_t)5, (idx_t)4);

    /* compute 5^(n-1) */
    seq_t pow5 = seq_pow((seq_t)5, n - (idx_t)1);

    /* result */
    return pow5 * bin;
}

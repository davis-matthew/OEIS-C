/*
 * A030506  Graham-Sloane-type lower bound on the size of a ternary (n,3,6)
 *          constant-weight code.
 * https://oeis.org/A030506
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(binomial(n,6) * 64 / (2*n + 1))
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 6)          /* undefined for n < 6 */
        return (seq_t)0;

    seq_t comb   = binomial(n, 6);
    seq_t num    = comb * (seq_t)64;
    seq_t denom  = (seq_t)(2 * n + 1);

    seq_t q = num / denom;
    seq_t r = num % denom;
    return (r == 0) ? q : q + 1;
}

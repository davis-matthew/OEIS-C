/*
 * A030509  Graham-Sloane-type lower bound on the size of a ternary (n,3,9)
 *          constant-weight code.
 * https://oeis.org/A030509
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(binomial(n,9) * 512 / (2*n + 1))
 * Range:     u8 none, u16 n <= 11, u32 n <= 28, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 9) return 0;           /* undefined below the offset */

    const idx_t w = 9;
    seq_t comb   = binomial(n, w);          /* C(n,9) */
    seq_t num    = comb * (seq_t)512;       /* numerator: C(n,9)*512 */
    seq_t denom  = (seq_t)((idx_t)2 * n + 1); /* denominator: 2*n+1 */

    seq_t res = num / denom;               /* floor part */
    if (num % denom != 0) ++res;           /* ceil correction */
    return res;
}

/*
 * A134763  a(n) = (1/2)*( (1+(-1)^n)*A134762(n/2) + 2*(1-(-1)^n) ).
 * https://oeis.org/A134763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*((1+(-1)^n)*A134762(n/2) + 2*(1-(-1)^n))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* odd indices are always 2 */
    if (n & (idx_t)1)
        return (seq_t)2;

    /* even indices: a(2k) = 3*C(2k,k) - 2 */
    idx_t k = n >> 1;              /* n/2 */
    seq_t v = binom(n, k);         /* binomial coefficient C(n, n/2) */
    v = v * (seq_t)3;              /* multiply by 3 */
    v = v - (seq_t)2;              /* subtract 2 */
    return v;
}

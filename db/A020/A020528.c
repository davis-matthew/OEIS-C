/*
 * A020528  3rd Bernoulli polynomial evaluated at powers of 2 (multiplied by
 *          6).
 * https://oeis.org/A020528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n+1)*(2^(n+1)-1)*(2^(n+1)-2)/4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute m = 2^(n+1) */
    seq_t m = pow2((idx_t)(n) + 1);

    /* product m*(m-1)*(m-2) */
    seq_t prod = m * (m - (seq_t)1);
    prod = prod * (m - (seq_t)2);

    /* divide by 4 then multiply by 3 */
    prod /= (seq_t)4;
    prod *= (seq_t)3;

    return prod;
}

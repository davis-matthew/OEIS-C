/*
 * A134057  a(n) = binomial(2^n-1,2).
 * https://oeis.org/A134057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2^n-1, 2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);          /* 2^n */
    seq_t k = p - 1;            /* 2^n - 1 */
    seq_t res = (k * (k - 1)) / ((seq_t)2); /* k*(k-1)/2 */
    return res;
}

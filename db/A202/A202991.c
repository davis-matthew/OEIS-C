/*
 * A202991  E.g.f: Sum_{n>=0} 3^(n^2) * exp(-2*3^n*x) * x^n/n!.
 * https://oeis.org/A202991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^n - 2)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* a(0) = (3^0 - 2)^0 = (-1)^0 = 1 */
    if (n == 0)
        return (seq_t)1;

    /* compute base = 3^n - 2 */
    seq_t base = pow3(n);
    base -= (seq_t)2;

    /* compute (3^n - 2)^n */
    return big_pow(base, n);
}

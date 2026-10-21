/*
 * A220812  Denominators of the Verhulst sequence x(n+1) = 4*x(n) - 3*x(n)^2,
 *          x(0) = 1/10.
 * https://oeis.org/A220812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 10^(2^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute exponent = 2^n */
    seq_t exp = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        exp *= (seq_t)2;

    /* base = 10 */
    seq_t base = (seq_t)10;

    /* return base^exp */
    return big_pow(base, exp);
}

/*
 * A170765  Expansion of g.f.: (1+x)/(1-45*x).
 * https://oeis.org/A170765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=46*45^(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* exponent is n-1, cast to seq_t because big_pow accepts seq_t exp */
    seq_t exp = (seq_t)(n - 1);
    seq_t pow45 = big_pow((seq_t)45, exp);
    return (seq_t)46 * pow45;
}

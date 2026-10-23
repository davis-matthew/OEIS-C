/*
 * A135561  a(n) = 2^A135560(n) - 1.
 * https://oeis.org/A135561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^e - 1 where e = trailing_zeros(n) + 1 + (n is a power
 *            of two? 1 : 0)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  For powers of two  :  e = log2(n) + 2
    For all other n     :  e = trailing_zeros(n) + 1   */

/* include helper that computes 2^exp as seq_t */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    idx_t tmp = n;
    idx_t tz  = 0;

    /* count trailing zeros of n */
    while ((tmp & (idx_t)1) == (idx_t)0)
    {
        tz++;
        tmp >>= (idx_t)1;
    }

    /* test if n is a power of two */
    bool is_pow2 = ((n & (n - (idx_t)1)) == (idx_t)0);

    /* exponent for the power of two */
    idx_t exp = tz + (idx_t)1 + (is_pow2 ? (idx_t)1 : (idx_t)0);

    /* compute 2^exp and subtract 1 */
    seq_t val = pow2(exp);          /* pow2 is provided by ../utils/pow2.h */
    return val - (seq_t)1;
}

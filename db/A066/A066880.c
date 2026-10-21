/*
 * A066880  Biased numbers: n such that all terms of the sequence f(n),
 *          f(f(n)), f(f(f(n))), ..., 1, where f(k) = floor(k/2), are odd.
 * https://oeis.org/A066880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^floor((n+3)/2) - 1 - (n mod 2)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute exponent: floor((n+3)/2) = (n/2) + 1 + (n mod 2) */
    idx_t exp = (n >> 1) + 1 + (n & 1);

    /* 2^exp */
    seq_t result = pow2(exp);

    /* Subtract 1 and (n mod 2) */
    result -= (seq_t)1;
    result -= (seq_t)(n & 1);

    return result;
}

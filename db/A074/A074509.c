/*
 * A074509  a(n) = 1^n + 3^n + 7^n.
 * https://oeis.org/A074509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 3^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses fast exponentiation via the utility
   function big_pow defined in ../utils/big_pow.h.  This works for
   all supported seq_t types, including arbitrary-precision.  */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    const seq_t one = (seq_t)1;
    /* Compute 3^n and 7^n using big_pow. The exponent type is seq_t,
       so cast n appropriately. */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    /* Sum the terms: 1^n is always 1. */
    return one + pow3 + pow7;
}

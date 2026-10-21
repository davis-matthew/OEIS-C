/*
 * A081421  a(n) = (3^(2*n) + 5^(2*n)) / 2.
 * https://oeis.org/A081421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^(2*n) + 5^(2*n)) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent is 2*n */
    seq_t exp = (seq_t)n * (seq_t)2;
    seq_t pow3 = big_pow((seq_t)3, exp);
    seq_t pow5 = big_pow((seq_t)5, exp);
    return (pow3 + pow5) / (seq_t)2;
}

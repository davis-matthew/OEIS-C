/*
 * A048469  a(n) = T(8,n), array T given by A047858.
 * https://oeis.org/A048469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(n+16) - 7
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
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
    /* n >= 1 */
    seq_t power = big_pow((seq_t)2, (seq_t)(n - 1));
    seq_t mult  = ((seq_t)n) + (seq_t)16;
    seq_t result = power * mult;
    return result - (seq_t)7;
}

/*
 * A168493  a(n) = 3^floor(n^2/2).
 * https://oeis.org/A168493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^floor(n^2/2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute floor(n^2/2) as an exponent of type idx_t */
    idx_t exp = (idx_t)n * (idx_t)n / 2;
    return seq_pow((seq_t)3, exp);
}

/*
 * A187879  a(n) = 3^(n^3).
 * https://oeis.org/A187879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n^3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent = n^3 */
    seq_t nn = (seq_t)n;
    seq_t exp = nn * nn * nn;
    return big_pow((seq_t)3, exp);
}

/*
 * A010813  25th powers: a(n) = n^25.
 * https://oeis.org/A010813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^25
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    return big_pow((seq_t)n, (seq_t)25);
}

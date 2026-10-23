/*
 * A134351  Binomial transform of [1, 5, -1, 5, -1, 5, ...]. Inverse binomial
 *          transform of A134350.
 * https://oeis.org/A134351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 2^(n+1) for n >= 1; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return seq_pow((seq_t)2, n + 1) + (seq_t)2;
}

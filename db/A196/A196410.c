/*
 * A196410  a(n) = n*2^(n-5).
 * https://oeis.org/A196410
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^(n-5)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)0;

    seq_t powv = big_pow((seq_t)2, (seq_t)(n - 5));
    return (seq_t)n * powv;
}

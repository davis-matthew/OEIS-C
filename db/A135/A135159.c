/*
 * A135159  a(n) = 5^n - 3^n + 2^n.
 * https://oeis.org/A135159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 3^n + 2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t five_pow  = big_pow((seq_t)5, (seq_t)n);
    seq_t three_pow = big_pow((seq_t)3, (seq_t)n);
    seq_t two_pow   = big_pow((seq_t)2, (seq_t)n);
    return five_pow - three_pow + two_pow;
}

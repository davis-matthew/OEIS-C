/*
 * A024065  a(n) = 6^n - n^3.
 * https://oeis.org/A024065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - n^3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow6 = seq_pow((seq_t)6, n);
    seq_t n3   = seq_pow((seq_t)n, 3);
    return pow6 - n3;
}

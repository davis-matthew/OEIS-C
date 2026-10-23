/*
 * A049294  Number of subgroups of index 3 in free group of rank n+1.
 * https://oeis.org/A049294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 3*6^n - 3*2^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow6 = seq_pow(6, n);
    seq_t pow2 = seq_pow(2, n);
    seq_t res = 3 * pow6 - 3 * pow2 + 1;
    return res;
}

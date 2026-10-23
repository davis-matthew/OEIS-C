/*
 * A049295  Number of subgroups of index 4 in free group of rank n+1.
 * https://oeis.org/A049295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*24^n - 4*6^n - 2*4^n + 4*2^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t p24 = seq_pow((seq_t)24, n);
    seq_t p6  = seq_pow((seq_t)6, n);
    seq_t p4  = seq_pow((seq_t)4, n);
    seq_t p2  = seq_pow((seq_t)2, n);

    seq_t res = 4 * p24;
    res -= 4 * p6;
    res -= 2 * p4;
    res += 4 * p2;
    res -= 1;
    return res;
}

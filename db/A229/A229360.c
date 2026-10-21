/*
 * A229360  Total sum of n-th powers of parts in all partitions of 9.
 * https://oeis.org/A229360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 67 + 26*2^n + 15*3^n + 8*4^n + 5*5^n + 3*6^n + 2*7^n +
 *            8^n + 9^n
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t res = 67;
    res += 26 * pow_uint(2, n);
    res += 15 * pow_uint(3, n);
    res += 8  * pow_uint(4, n);
    res += 5  * pow_uint(5, n);
    res += 3  * pow_uint(6, n);
    res += 2  * pow_uint(7, n);
    res += pow_uint(8, n);
    res += pow_uint(9, n);
    return res;
}

/*
 * A145037  Number of 1's minus number of 0's in the binary representation of
 *          n.
 * https://oeis.org/A145037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = Sum_{i=1..k} (2*b[i] - 1) where b[i] are the bits of n (0
 *            or 1) and k is the number of bits in the binary expansion of n.
 *            For n = 0 this sum is empty and a(0) = 0
 * Range:     i8 n <= 9721, i16 n <= 9721, i32 n <= 9721, i64 n <= 9721,
 *            i128 n <= 9721, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t tmp = n;
    seq_t ones = 0;
    seq_t bits = 0;
    while (tmp)
    {
        bits++;
        ones += tmp & 1ULL;
        tmp >>= 1;
    }
    return (seq_t)(ones * 2 - bits);
}

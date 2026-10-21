/*
 * A211715  Number of (n+1) X (n+1) -11..11 symmetric matrices with every 2 X
 *          2 subblock having sum zero and two or four distinct values.
 * https://oeis.org/A211715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 + 3*2^(n+2)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    return (seq_t)10 + (seq_t)3 * pow2(n + 2);
}

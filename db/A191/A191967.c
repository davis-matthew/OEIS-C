/*
 * A191967  n * (numbers that are not divisible by 3).
 * https://oeis.org/A191967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * A001651(n) = n * ((3*n - (n%2?1:2))/2)
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn   = (seq_t)n;
    seq_t flag = (n & 1) ? 1 : 2;                /* 1 for odd n, 2 for even n */
    seq_t t    = ((seq_t)3 * nn - flag) / 2;     /* A001651(n) */
    return nn * t;                              /* n * A001651(n) */
}

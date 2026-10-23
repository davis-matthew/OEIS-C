/*
 * A016857  a(n) = (5n)^9.
 * https://oeis.org/A016857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t base = 5 * (seq_t)n;   /* base = 5n */
    seq_t res  = 1;              /* res = 1 */
    for (int i = 0; i < 9; ++i)  /* multiply 9 times to get (5n)^9 */
        res *= base;
    return res;
}

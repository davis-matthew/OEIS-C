/*
 * A013835  a(n) = 5^(5*n + 2).
 * https://oeis.org/A013835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(5*n + 2) = 25 * 3125^n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)25;
    for (idx_t i = 0; i < n; ++i)
        res *= (seq_t)3125;
    return res;
}

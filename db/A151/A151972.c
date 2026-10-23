/*
 * A151972  Numbers that are congruent to {0, 1, 6, 10} mod 15.
 * https://oeis.org/A151972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15 * floor((n-1)/4) + [0, 1, 6, 10][(n-1)%4]
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t offsets[4] = { (seq_t)0, (seq_t)1, (seq_t)6, (seq_t)10 };

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n==0, return 0 */

    idx_t q = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    return (seq_t)15 * (seq_t)q + offsets[r];
}

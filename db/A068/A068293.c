/*
 * A068293  a(1) = 1; thereafter a(n) = 6*(2^(n-1) - 1).
 * https://oeis.org/A068293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 6 * (2^(n-1) - 1) for n >= 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t v = 1;
    /* compute 2^(n-1) */
    for (idx_t i = 0; i < n - 1; ++i)
        v <<= 1;

    v -= (seq_t)1; /* 2^(n-1) - 1 */
    v *= (seq_t)6; /* times 6 */
    return v;
}

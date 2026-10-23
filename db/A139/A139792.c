/*
 * A139792  First quadrisection of A139763 (1, 2, 3, 4, 11, ...).
 * https://oeis.org/A139792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 16*a(n-1) - 5 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 1;              /* a(0) */
    if (n == 0) return val;

    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * (seq_t)16 - (seq_t)5;   /* recurrence */
    }
    return val;
}

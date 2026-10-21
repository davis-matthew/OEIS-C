/*
 * A219165  Recurrence equation a(n+1) = a(n)^4 - 4*a(n)^2 + 2 with a(0) = 6.
 * https://oeis.org/A219165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 6, a(n+1) = a(n)^4 - 4*a(n)^2 + 2
   Works for any idx_t ≥ 0. */
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)6;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t sq = val * val;               /* val^2 */
        seq_t fourth = sq * sq;             /* val^4 */
        seq_t four_sq = (seq_t)4 * sq;      /* 4 * val^2 */
        val = fourth - four_sq + (seq_t)2;  /* recurrence */
    }
    return val;
}

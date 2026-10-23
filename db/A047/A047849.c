/*
 * A047849  a(n) = (4^n + 2)/3.
 * https://oeis.org/A047849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = (4^n + 2)/3 Recurrence: a(0) = 1, a(n) = 4*a(n-1) - 2
 *            (valid for all n ≥ 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Start from a(0) */
    seq_t value = (seq_t)1;

    /* Iterate the recurrence up to the requested n */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* value = 4 * value - 2 */
        value = value * (seq_t)4;
        value = value - (seq_t)2;
    }

    return value;
}

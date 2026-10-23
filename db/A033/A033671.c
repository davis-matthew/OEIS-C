/*
 * A033671  Trajectory of 59 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + rev_decimal(a(n-1)), a(0) = 59
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 84,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* reverse_decimal is defined in the utils library. */
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)59;               /* a(0) */
    if (n == 0)
        return x;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t rev = reverse_decimal(x);
        x = x + rev;
    }
    return x;
}

/*
 * A033672  Trajectory of 69 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 37, u128 n <= 81,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=69; a(n+1)=a(n)+reverse_decimal(a(n)) */
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)69;          /* a(0) */
    for (idx_t i = 0; i < n; ++i) /* perform n iterations of the map */
        x = x + reverse_decimal(x);
    return x;
}

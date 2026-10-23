/*
 * A008461  Take sum of squares of digits of previous term.
 * https://oeis.org/A008461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   digit_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 7; for n ≥ 2, a(n) = sum of squares of digits of a(n-1).  For n > 5, a(n) = 1.  */

#include <stdint.h>
#include "../utils/digit_square_sum.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)7;

    seq_t val = 7;
    /* We only need up to the 5th transformation; after that the value is always 1. */
    idx_t steps = (n < 5 ? n - 1 : 4);  /* number of digit‑square‑sum steps to apply */
    for (idx_t i = 0; i < steps; ++i)
        val = (seq_t)digit_square_sum((uint64_t)val);

    return (n > 5 ? (seq_t)1 : val);
}

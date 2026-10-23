/*
 * A021381  Decimal expansion of 1/377.
 * https://oeis.org/A021381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let r_0 = 1.
   For n >= 0:
        a(n) = floor((10 * r_n) / 377);
        r_{n+1} = (10 * r_n) mod 377.
   a(0) is the first decimal digit after the point of 1/377. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* remainder of the division 1 / 377 after n steps
       (stored in a 64‑bit integer to avoid overflow) */
    uint64_t rem = 1;
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        digit = (seq_t)((rem * 10) / 377ULL);
        rem   = (rem * 10) % 377ULL;
    }

    return digit;
}

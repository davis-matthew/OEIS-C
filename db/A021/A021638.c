/*
 * A021638  Decimal expansion of 1/634.
 * https://oeis.org/A021638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/634) mod 10 (the n‑th decimal digit of
 *            1/634)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t r = 1;                 /* initial remainder: 1/634 */
    for (idx_t i = 0; i < n; ++i) { /* advance to the next digit */
        r = (r * 10) % 634;
    }
    return (seq_t)((r * 10) / 634);  /* digit at position n */
}

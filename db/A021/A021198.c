/*
 * A021198  Decimal expansion of 1/194.
 * https://oeis.org/A021198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit of 1/194 at position n (0‑based)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the n-th decimal digit of 1/194 by simulated division. */
    uint64_t rem = 1;          /* remainder after the decimal point */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;             /* bring down the next digit */
        uint64_t digit = rem / 194;   /* integer part is the desired digit */
        rem = rem % 194;        /* new remainder for next step */
        if (i == n)
            return (seq_t)digit;
    }
    return 0;  /* unreachable */
}

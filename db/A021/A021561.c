/*
 * A021561  Decimal expansion of 1/557.
 * https://oeis.org/A021561
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

/* Sequence: decimal digits of 1/557 (after the decimal point) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The decimal expansion of 1/557 is produced by the standard long‑division
       algorithm.  The remainder r is always < 557, so all intermediate values
       fit easily in a 32‑bit integer.  The digit computed at step i is the
       quotient of r*10 and 557, and the new remainder is r*10 % 557. */
    uint32_t r = 1;           /* start with numerator 1 */
    uint32_t d = 0;
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;
        d = r / 557;
        r = r % 557;
        if (i == n)
            return (seq_t)d;
    }
    return 0; /* unreachable, keeps compiler happy */
}

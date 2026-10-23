/*
 * A021757  Decimal expansion of 1/753.
 * https://oeis.org/A021757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digit of 1/753: a(n) = floor( (10 * (10^n mod 753)) / 753 ) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* remainder r after n divisions before producing a(n) */
    uint64_t r = 1;          /* initial remainder 1/753 */

    for (idx_t i = 0; i < n; ++i) {
        /* simulate r = (r * 10) % 753 at each step */
        r = (r * 10) % 753;
    }

    /* digit at position n */
    return (seq_t)((r * 10) / 753);
}

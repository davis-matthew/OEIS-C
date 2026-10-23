/*
 * A021783  Decimal expansion of 1/779.
 * https://oeis.org/A021783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/779) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here a(n) is the n‑th decimal digit of 1/779, with a(0) = 0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* We perform ordinary long division of 1 by 779, emitting digits
       one by one.  The remainder never exceeds 778, so 32‑bit arithmetic
       is safe. */
    unsigned int rem = 1;          /* current remainder (initially numerator) */
    unsigned int digit = 0;        /* last computed digit */

    for (idx_t i = 0; i <= n; ++i) {
        unsigned int prod = rem * 10U;   /* shift left by one decimal place */
        digit = prod / 779U;             /* next decimal digit */
        rem   = prod % 779U;             /* new remainder */
    }

    return (seq_t)digit;
}

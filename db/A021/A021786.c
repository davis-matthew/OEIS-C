/*
 * A021786  Decimal expansion of 1/782.
 * https://oeis.org/A021786
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

/* Compute a(n) as the n-th digit after the decimal point in 1/782,
   using long division. */
#include <stdint.h>

/* a(n) returns the nth decimal digit of 1 / 782
   (a(0) = first digit after the point). */
seq_t a(idx_t n)
{
    const idx_t DEN = (idx_t)782;   /* denominator */
    idx_t rem = 1;                  /* remainder starts with numerator 1 */

    /* Advance remainder n times to reach the digit of interest */
    for (idx_t i = 0; i < n; ++i) {
        rem = (rem * 10) % DEN;     /* keep only the remainder */
    }
    /* Final step: compute the desired digit */
    rem *= 10;
    return (seq_t)(rem / DEN);
}

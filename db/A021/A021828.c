/*
 * A021828  Decimal expansion of 1/824.
 * https://oeis.org/A021828
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

/* Sequence A021828: decimal digit of 1/824 at position n after the decimal point
   in 0‑based indexing.  Equivalent to
   a(n) = floor(10^(n+1) / 824) mod 10. */
/* This implementation uses the long division algorithm which runs in O(n)
   time and needs only O(1) additional space. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* reminder of current remainder in long division of 1 by 824.
       Initially the remainder is the numerator 1. */
    uint64_t rem = 1;
    uint64_t digit = 0;

    /* Perform n+1 steps of the standard long‑division algorithm.
       After each step:
         rem *= 10;
         digit = rem / 824;   // integer digit
         rem   = rem % 824;   // new remainder
       The digit computed in the final step is a(n). */
    for (idx_t i = 0; i <= n; ++i) {
        rem   *= 10ULL;
        digit  = rem / 824ULL;   /* 0..9 */
        rem    = rem % 824ULL;
    }

    return (seq_t)digit;
}

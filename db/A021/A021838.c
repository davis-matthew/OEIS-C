/*
 * A021838  Decimal expansion of 1/834.
 * https://oeis.org/A021838
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

/* Sequence: decimal expansion of 1/834.
   a(n) is the nth digit after the decimal point of 0.001198...
   Computed by long division: repeatedly multiply the remainder by 10
   and take the quotient and remainder modulo 834. */

#include <stdbool.h>   /* for bool, if needed by build */

seq_t a(idx_t n)
{
    /* 1/834 has a simple periodic decimal expansion.
       We generate the digits by the textbook long‑division
       algorithm.  The remainder never exceeds 833, so a
       narrow unsigned int is sufficient for the intermediate
       computation, regardless of the size of seq_t. */
    unsigned int rem = 1;              /* remainder before the current digit */
    unsigned int digit = 0;             /* current digit (0…9) */
    idx_t i;

    for (i = 0; i <= n; ++i) {
        /* Multiply the remainder by 10 (like shifting the next
           decimal digit into place), then divide by 834 to
           obtain the next digit.  The new remainder is the
           modulus. */
        unsigned int mul = rem * 10u;
        digit = mul / 834u;
        rem   = mul % 834u;
    }

    return (seq_t)digit;
}

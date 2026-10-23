/*
 * A021887  Decimal expansion of 1/883.
 * https://oeis.org/A021887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit after the decimal point in the decimal
 *            expansion of 1/883. The sequence starts with a(0) = 0, a(1) = 0,
 *            a(2) = 1, a(3) = 1, a(4) = 3,
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The function computes the digit at position n of 1/883 using
   simple long division.  It runs in O(n) time and uses only
   integer arithmetic, satisfying the constraints given. */
seq_t a(idx_t n)
{
    seq_t rem = (seq_t)1;   /* starting remainder: numerators of 1/883 */
    seq_t digit = 0;

    /* Perform long division up to and including the nth digit. */
    for (idx_t i = 0; i <= n; i++) {
        rem *= (seq_t)10;              /* shift decimal one place */
        digit = rem / (seq_t)883;      /* integer division gives next digit */
        rem = rem % (seq_t)883;        /* remainder for next step */
    }
    return digit;   /* digit now holds a(n) */
}

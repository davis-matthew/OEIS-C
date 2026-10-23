/*
 * A021243  Decimal expansion of 1/239.
 * https://oeis.org/A021243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/239) mod 10 (decimal digit of 1/239)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Computes the nth decimal digit of 1/239 in sequence A021243.
   idx_t is an unsigned integer type (uint64_t by definition).
   seq_t is a (possibly arbitrary‑precision) integer class that supports
   + - * / % and comparison operators. */
seq_t a(idx_t n)
{
    seq_t rem = (seq_t)1;          /* initial remainder for 1/239 */
    seq_t digit;                   /* last digit produced */

    /* Perform n+1 steps of long division to reach the nth digit. */
    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * (seq_t)10;               /* shift left one decimal place */
        digit = rem / (seq_t)239;            /* next decimal digit */
        rem = rem % (seq_t)239;              /* new remainder for next iteration */
    }

    return digit;
}

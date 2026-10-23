/*
 * A021921  Decimal expansion of 1/917.
 * https://oeis.org/A021921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the (n+1)-st decimal digit of the fraction 1/917,
 *            computed by the standard long‑division remainder method
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term (0‑based) of the decimal expansion of 1/917. */
seq_t a(idx_t n)
{
    /* The denominator of the fraction. */
    const seq_t denom = (seq_t)917;

    /* Current remainder starting with 1/917 = 1 divided by 917. */
    seq_t remainder = (seq_t)1;

    /* Iterate until the desired position is reached. */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t tmp   = remainder * (seq_t)10; /* Multiply by 10 for next digit. */
        seq_t digit = tmp / denom;           /* Next decimal digit. */
        if (i == n)
            return digit;                   /* Found the requested digit. */
        remainder = tmp % denom;            /* Remainder for next iteration. */
    }

    /* In case n is negative or other unforeseen input; should not occur. */
    return (seq_t)0;
}

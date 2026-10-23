/*
 * A021107  Decimal expansion of 1/103.
 * https://oeis.org/A021107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digit of 1/103 (0-indexed).  a(n) is the n-th digit after the decimal point. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a simple long‑division algorithm to generate the n-th digit.
       The divisor is 103, which is prime, so the period is 102.  A
       direct simulation is sufficient for n up to 10000. */
    idx_t i = 0;
    seq_t rem = 1;      /* initial remainder */
    seq_t digit = 0;

    /* Perform the division n+1 times to obtain the n‑th digit. */
    do
    {
        rem *= (seq_t)10;
        digit = rem / (seq_t)103;
        rem %= (seq_t)103;
    } while (i++ < n);

    return digit;
}

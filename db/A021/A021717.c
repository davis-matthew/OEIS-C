/*
 * A021717  Decimal expansion of 1/713.
 * https://oeis.org/A021717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point in 1/713 (0‑based
 *            index)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Periodic decimal expansion of 1/713.
       The period length is the multiplicative order of 10 modulo 713,
       which is at most 660.  We pre‑compute the digits once and reuse them. */
    static idx_t period = 0;          /* number of digits in the period */
    static seq_t digits[700];        /* buffer for the period digits (enough for 660) */

    if (period == 0)                  /* first call – compute the period */
    {
        uint64_t r = 1;                /* remainder before any digit */
        idx_t i = 0;                   /* current position in the period */
        while (1)
        {
            r *= 10;
            int d = r / 713;          /* next decimal digit */
            digits[i] = (seq_t)d;     /* store it */
            r %= 713;                 /* new remainder */
            i++;
            if (r == 1)               /* cycle repeats */
            {
                period = i;           /* period length */
                break;
            }
        }
    }

    idx_t idx = n % period;           /* index within the period */
    return digits[idx];
}

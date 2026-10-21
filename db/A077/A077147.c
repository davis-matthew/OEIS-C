/*
 * A077147  a(n) = floor({concatenation 123 ... up to n}/n).
 * https://oeis.org/A077147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat(1,2,...,n) / n ) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is defined; for n==0 return 0 (not used by the sequence) */
    if (n == 0) return 0;

    /* Threshold where the number of decimal digits of the appended term
       increases. Initially it is 10 (when i reaches 10 we need 2 digits). */
    idx_t next_digit_boundary = 10;
    int digits = 1;                 /* current number of digits in i   */
    seq_t pow10 = 10;               /* 10^digits, starts at 10^1        */

    seq_t q = 0;                    /* running quotient   floor(conc/i)   */
    seq_t r = 0;                    /* current remainder of the big number modulo n */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* When i reaches the next digit boundary we need one more power
           of ten for the next term. */
        if (i == next_digit_boundary)
        {
            ++digits;
            next_digit_boundary *= 10;
            pow10 *= 10;            /* pow10 now 10^digits             */
        }

        /* Append the current integer i:
           cur = cur * 10^digits + i      (performed modulo n for r,
                                            full value for q)          */
        seq_t temp = r * pow10 + (seq_t)i;
        seq_t div   = temp / (seq_t)n;
        r          = temp % (seq_t)n;

        /* Update the quotient with the new digit(s). */
        seq_t mult = q * pow10 + div;
        q = mult;
    }

    return q;
}

/*
 * A225761  Numerators of the sums of reciprocals of the Collatz (3x+1)
 *          sequence beginning with n and stopping at 1.
 * https://oeis.org/A225761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225761: a(n) is the numerator of
   the sum 1/x(1) + 1/x(2) + … + 1/x(L) of the Collatz
   (3x+1) sequence that starts at n and ends at 1  */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥1 in the OEIS definition. */
    if (n == 0)  /* defensive: not defined for 0 */
        return 0;

    seq_t num = 0;          /* current numerator */
    seq_t den = 1;          /* current denominator */
    seq_t cur = (seq_t)n;   /* current element of Collatz sequence */

    while (1)
    {
        /* Add 1/cur to the running sum num/den.  */
        seq_t g  = gcd(den, cur);          /* gcd(den, cur)        */
        seq_t fact = cur / g;              /* cur/g to keep numbers small */
        seq_t new_num = num * fact + den / g;
        seq_t new_den = (den / g) * cur;   /* (den/g)*cur          */

        /* Reduce the fraction.  */
        seq_t g2  = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;

        /* If we have processed the last element, exit.  */
        if (cur == 1) break;

        /* Next Collatz step: 3x+1 if odd, else x/2.  */
        if ((cur & 1) != 0)
            cur = cur * 3 + 1;
        else
            cur = cur >> 1;
    }

    return num;   /* the required numerator */
}

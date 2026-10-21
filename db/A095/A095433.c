/*
 * A095433  a(n) = 18 written in base n.
 * https://oeis.org/A095433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal value of the base‑n representation of 18 (i.e.,
 *            18 expressed in base n, then read as a decimal integer)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset is 1; n>=1 by OEIS specification.  */
    if (n == 0)
        return 0;          /* Undefined for n==0; guard against wrap around. */

    /* Base 1: unary representation of 18 is sixteen 1's?   */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 18; ++i)   /* 18 ones */
            res = res * 10 + 1;
        return res;
    }

    /* For bases >= 19 the value 18 is a single digit less than the base,
       so the representation is simply the decimal number 18. */
    if (n >= 19)
        return 18;

    /* Convert 18 to base n and build the decimal number by concatenating
       the digits. */
    seq_t val = 18;
    seq_t res = 0;
    seq_t pow10 = 1;          /* 10^position */

    while (val > 0) {
        seq_t digit = val % n;
        val /= n;
        res += digit * pow10;
        pow10 *= 10;
    }

    return res;
}

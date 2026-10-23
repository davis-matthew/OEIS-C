/*
 * A018857  Smallest power of 3 that begins with n.
 * https://oeis.org/A018857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest power of 3 whose decimal * representation
 *            starts with the digits of n. * The function searches exponents e
 *            = 0,1,2,…, checking the * leading digits of 3^e until a match is
 *            found
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 32,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* number of decimal digits of the index n   */
    idx_t d = digit_len((seq_t)n);
    seq_t cur = (seq_t)1;            /* 3^0 */

    for (;;)
    {
        idx_t cur_digits = digit_len(cur);

        if (cur_digits >= d)
        {
            idx_t shift = cur_digits - d;     /* power of 10 to trim trailing digits */
            seq_t divisor = pow10(shift);     /* 10^(cur_digits-d) */
            seq_t leading = cur / divisor;    /* most significant d digits of cur */

            if (leading == (seq_t)n)
                return cur;                    /* found minimal exponent e */
        }

        cur = cur * (seq_t)3;          /* next power of 3 */
    }
}

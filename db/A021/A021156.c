/*
 * A021156  Decimal expansion of 1/152.
 * https://oeis.org/A021156
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

/* Decimal expansion of 1/152
   a(0) = 0  (integer part)
   For n ≥ 1, a(n) = the (n+1)-th decimal digit of 1/152
   (i.e., the digit after the first decimal ‘0’).  This follows the
   OEIS definition, so that a(1)=0, a(2)=6, a(3)=5, ….
   The digits are computed by standard long‑division. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* denominator of 1/152 */
    const seq_t den = 152;

    /* a(0) is the integer part */
    if (n == 0)
        return (seq_t)0;

    /* remainder before the first decimal digit */
    seq_t r = 1;

    /* perform long division n+1 times to obtain the (n+1)-th digit */
    idx_t i;
    for (i = 1; i <= n + 1; ++i) {
        r = r * (seq_t)10;          /* bring the next decimal place */
        seq_t d = r / den;          /* the digit at this place */
        r = r % den;                /* new remainder for next step */

        if (i == n + 1)
            return d;
    }

    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}

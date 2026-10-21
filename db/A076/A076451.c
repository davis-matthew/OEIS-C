/*
 * A076451  Let w(1)=w(2)=w(3)=1, w(n) =
 *          (-1)^floor(n/2)*sign(w(n-1)-w(n-2))*w(n-3), then a(n) = 1+w(n).
 * https://oeis.org/A076451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + w(n) Recurrence for w: w(1) = w(2) = w(3) = 1 w(n) =
 *            (-1)^(⌊n/2⌋) * sign( w(n-1) - w(n-2) ) * w(n-3) for n ≥ 4 where
 *            sign(x) = 1 if x>0, -1 if x<0, 0 if x==0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the small indices directly. */
    if (n <= 3)
        return (seq_t)2;           /* a(1)=a(2)=a(3)=2 */

    /* w(1), w(2), w(3).  All values stay in { -1, 0, 1 }. */
    signed int w_nm3 = 1;          /* w(n-3)  */
    signed int w_nm2 = 1;          /* w(n-2)  */
    signed int w_nm1 = 1;          /* w(n-1)  */
    signed int w_n   = 0;          /* current w(n) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Difference w(i-1) - w(i-2). */
        int diff = w_nm1 - w_nm2;
        int sign = (diff > 0) - (diff < 0);   /* sign function */

        /* (-1)^(floor(i/2)).  floor(i/2) & 1 == 0 ⇒ even ⇒ +1. */
        int factor = (((i / 2) & 1) == 0) ? 1 : -1;

        w_n = factor * sign * w_nm3;

        /* Shift variables for next iteration. */
        w_nm3 = w_nm2;
        w_nm2 = w_nm1;
        w_nm1 = w_n;
    }

    return (seq_t)(1 + w_n);
}

/*
 * A042011  Denominators of continued fraction convergents to sqrt(528).
 * https://oeis.org/A042011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 46*a(n-2) - a(n-4) for n > 3 Initial values: a(0)=1,
 *            a(1)=1, a(2)=45, a(3)=46
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 45;
    if (n == 3) return 46;

    /* Use a circular buffer of length 4 to keep only the needed
       previous values.  Buffer indices correspond to n modulo 4. */
    seq_t buf[4] = { 1, 1, 45, 46 };

    idx_t i;
    for (i = 4; i <= n; ++i)
    {
        /* Compute a(i) using the recurrence and store it in the
           slot that will be reused on the next round. */
        buf[i & 3] = 46 * buf[(i - 2) & 3] - buf[(i - 4) & 3];
    }

    /* The requested term is now in the slot corresponding to n mod 4. */
    return buf[n & 3];
}

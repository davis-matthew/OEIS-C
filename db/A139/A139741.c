/*
 * A139741  a(n) = 11^n - 3^n.
 * https://oeis.org/A139741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 3^n Recurrence: a(0)=0, a(1)=8,
 *            a(n)=14*a(n-1)-33*a(n-2) for n≥2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) = 0 */
    if (n == 1)
        return (seq_t)8;          /* a(1) = 8 */

    /* iterate using the recurrence.  a0 = a(k-2), a1 = a(k-1) */
    seq_t a0 = (seq_t)0;          /* a(0) */
    seq_t a1 = (seq_t)8;          /* a(1) */
    seq_t ai = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        ai = (seq_t)14 * a1 - (seq_t)33 * a0;  /* a(i) = 14*a(i-1) - 33*a(i-2) */
        a0 = a1;
        a1 = ai;
    }
    return ai;     /* after the loop, ai holds a(n) */
}

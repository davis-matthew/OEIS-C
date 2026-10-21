/*
 * A089513  Third column of triangle A089504.
 * https://oeis.org/A089513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 90*a(n-1) - 1944*a(n-2) + 8640*a(n-3) (n>=3) initial
 *            values: a(0)=1, a(1)=90, a(2)=6156
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0)
        return 1;
    if (n == 1)
        return 90;
    if (n == 2)
        return 6156;

    /* Use the linear recurrence: a(n) = 90*a(n-1) - 1944*a(n-2) + 8640*a(n-3). */
    seq_t a0 = 1;     /* a(n-3) */
    seq_t a1 = 90;    /* a(n-2) */
    seq_t a2 = 6156;  /* a(n-1) */
    idx_t i;

    for (i = 3; i <= n; ++i)
    {
        seq_t cur = a2 * 90 - a1 * 1944 + a0 * 8640;
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }

    return a2;
}

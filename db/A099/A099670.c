/*
 * A099670  Partial sums of repdigits of A002277.
 * https://oeis.org/A099670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 21*a(n-2) + 10*a(n-3) for n > 3 initial
 *            values: a(1) = 3, a(2) = 36, a(3) = 369
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1, 0 is unused */

    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)36;
    if (n == 3)
        return (seq_t)369;

    /* use only the last three terms – O(n) time, O(1) memory */
    seq_t a1 = (seq_t)3;   /* a(n-3)   */
    seq_t a2 = (seq_t)36;  /* a(n-2)   */
    seq_t a3 = (seq_t)369; /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* next = 12*a3 + 10*a1 - 21*a2  (parentheses ensure positive side first) */
        seq_t next = (seq_t)12 * a3 + (seq_t)10 * a1 - (seq_t)21 * a2;

        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

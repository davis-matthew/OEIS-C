/*
 * A174685  Indices i such that 2*A000326(i)+1 is also a pentagonal number.
 * https://oeis.org/A174685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence a(n) =
 *            a(n-1)+1154*a(n-2)-1154*a(n-3)-a(n-4)+a(n-5), with initial
 *            values a(1)=0, a(2)=75, a(3)=244, a(4)=86359, a(5)=281384
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, used by seq_alloc_array if needed */

seq_t a(idx_t n)
{
    /* Return early for the first five terms (offset 1). */
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)75;
    if (n == (idx_t)3) return (seq_t)244;
    if (n == (idx_t)4) return (seq_t)86359;
    if (n == (idx_t)5) return (seq_t)281384;

    /* Keep a sliding window of the last five terms. */
    seq_t a1 = (seq_t)0;          /* a(k-5) */
    seq_t a2 = (seq_t)75;         /* a(k-4) */
    seq_t a3 = (seq_t)244;        /* a(k-3) */
    seq_t a4 = (seq_t)86359;      /* a(k-2) */
    seq_t a5 = (seq_t)281384;     /* a(k-1) */

    /* Compute terms up to a(n). */
    for (idx_t k = (idx_t)6; k <= n; ++k)
    {
        /* Compute new = a5 + 1154*a4 - 1154*a3 - a2 + a1
           in an order that keeps intermediate results positive. */
        seq_t temp = a5 + 1154 * a4;
        temp += a1;                     /* temp = a5 + 1154*a4 + a1 */
        seq_t sub  = 1154 * a3;
        sub += a2;                      /* sub  = 1154*a3 + a2 */
        seq_t newval = temp - sub;       /* newval = a(k) */

        /* Slide the window: a1 <- a2, a2 <- a3, a3 <- a4, a4 <- a5, a5 <- newval */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = newval;
    }

    /* After the loop a5 holds a(n). */
    return a5;
}

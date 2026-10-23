/*
 * A124281  Expansion of 1/(1-x-2*x^2+2*x^3-2*x^4).
 * https://oeis.org/A124281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=3
 *            a(n)=a(n-1)+2*a(n-2)-2*a(n-3)+2*a(n-4) for n>=4
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)3;

    /* Sliding window of the last four terms */
    seq_t a0 = (seq_t)1;  /* a(n-4) */
    seq_t a1 = (seq_t)1;  /* a(n-3) */
    seq_t a2 = (seq_t)3;  /* a(n-2) */
    seq_t a3 = (seq_t)3;  /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 + ((a2 << 1) + (a0 << 1));
        an -= (a1 << 1);
        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}

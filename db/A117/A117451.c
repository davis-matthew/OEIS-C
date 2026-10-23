/*
 * A117451  Expansion of (1-x+x^2+x^5)/((1-x)*(1-x^5)).
 * https://oeis.org/A117451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) with initial values a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=3
 * Range:     u8 n <= 639, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the known small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)1;
    if (n == 5) return (seq_t)3;

    /* Recurrence requires values from n-6 to n-1.
       We keep them in a fixed‑size ring buffer of length 6. */
    seq_t last[6];
    last[0] = (seq_t)1;   /* a(0) */
    last[1] = (seq_t)0;   /* a(1) */
    last[2] = (seq_t)1;   /* a(2) */
    last[3] = (seq_t)1;   /* a(3) */
    last[4] = (seq_t)1;   /* a(4) */
    last[5] = (seq_t)3;   /* a(5) */

    for (idx_t i = 6; i <= n; ++i)
    {
        /* a(i) = a(i-1) + a(i-5) - a(i-6)
           a(i-1) == last[5], a(i-5) == last[1], a(i-6) == last[0] */
        seq_t cur = last[5] + last[1] - last[0];

        /* shift the window: discard a(i-6), shift left, append cur. */
        for (idx_t j = 0; j < 5; ++j)
            last[j] = last[j + 1];
        last[5] = cur;
    }

    return last[5];   /* after the loop last[5] holds a(n) */
}

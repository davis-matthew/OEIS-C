/*
 * A204377  Number of (n+2) X 6 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..1 introduced in
 *          row major order.
 * https://oeis.org/A204377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4)-a(n-5) (n>8) * Initial
 *            values: * a(1)=113, a(2)=63, a(3)=75, a(4)=87, * a(5)=111,
 *            a(6)=153, a(7)=217, a(8)=323
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 8 base values (indices 1..8) – 0‑based array */ 
    static const seq_t init[8] =
    {
        (seq_t)113,  (seq_t)63,  (seq_t)75,  (seq_t)87,
        (seq_t)111,  (seq_t)153, (seq_t)217, (seq_t)323
    };

    if (n < 1u)   /* undefined for non‑positive indices */
        return (seq_t)0;

    if (n <= 8u)  /* return one of the stored base values */
        return init[n-1u];

    /* When n > 8, keep only the last five terms of the sequence.
       recent[0] ... recent[4] correspond to
          a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) respectively. */
    seq_t recent[5] =
    {
        init[3u],  /* a(4)  */
        init[4u],  /* a(5)  */
        init[5u],  /* a(6)  */
        init[6u],  /* a(7)  */
        init[7u]   /* a(8)  */
    };

    seq_t newv;
    for (idx_t i = 9u; i <= n; ++i) {
        newv = recent[4] + recent[3] + recent[2];
        newv -= recent[1];
        newv -= recent[0];

        /* shift the window, drop the oldest term */
        recent[0] = recent[1];
        recent[1] = recent[2];
        recent[2] = recent[3];
        recent[3] = recent[4];
        recent[4] = newv;
    }

    return recent[4];   /* a(n) is the most recent value after the loop */
}

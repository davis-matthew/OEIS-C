/*
 * A028097  Expansion of 1/((1-3x)(1-7x)(1-10x)(1-12x)).
 * https://oeis.org/A028097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*a(n-1) - 361*a(n-2) + 1662*a(n-3) - 2520*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (recurrence derived from 1/((1-3x)(1-7x)(1-10x)(1-12x)))    */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)32;
    if (n == (idx_t)2) return (seq_t)663;
    if (n == (idx_t)3) return (seq_t)11326;

    /* Keep the last four values only. */
    seq_t a0 = (seq_t)1;       /* a(n-4)  */
    seq_t a1 = (seq_t)32;      /* a(n-3)  */
    seq_t a2 = (seq_t)663;     /* a(n-2)  */
    seq_t a3 = (seq_t)11326;   /* a(n-1)  */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute a(i) = 32*a3 - 361*a2 + 1662*a1 - 2520*a0.
           Arrangement used guarantees no unsigned underflow. */
        an = (seq_t)32 * a3;
        an += (seq_t)1662 * a1;
        an -= (seq_t)361 * a2;
        an -= (seq_t)2520 * a0;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;   /* a3 holds a(n) after loop finishes */
}

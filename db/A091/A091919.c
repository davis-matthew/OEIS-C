/*
 * A091919  Expansion of 1/((1-2*x)*(1-x^2)^2).
 * https://oeis.org/A091919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A091919
    Expansion of 1/((1-2*x)*(1-x^2)^2).

    Recurrence:
        a(0)=1, a(1)=2, a(2)=6, a(3)=12, a(4)=27
        a(n)=2*a(n-1)+2*a(n-2)-4*a(n-3)-a(n-4)+2*a(n-5)   for n≥5
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)27;

    /* Keep only the last five terms of the recurrence */
    seq_t t0 = (seq_t)1;   /* a(n-5) */
    seq_t t1 = (seq_t)2;   /* a(n-4) */
    seq_t t2 = (seq_t)6;   /* a(n-3) */
    seq_t t3 = (seq_t)12;  /* a(n-2) */
    seq_t t4 = (seq_t)27;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (t4 * (seq_t)2)          // 2*a(n-1)
                   + (t3 * (seq_t)2)          // 2*a(n-2)
                   - (t2 * (seq_t)4)          // -4*a(n-3)
                   - (t1)                    // -a(n-4)
                   + (t0 * (seq_t)2);        // +2*a(n-5)

        /* Shift the terms for the next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }

    return t4;
}

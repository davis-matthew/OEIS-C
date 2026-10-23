/*
 * A181296  The number of odd entries in all the 2-compositions of n.
 * https://oeis.org/A181296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies
 *            a(n)=7*a(n-1)-12*a(n-2)-4*a(n-3)+12*a(n-4)-4*a(n-5), with
 *            a(0)=0, a(1)=2, a(2)=10, a(3)=48, a(4)=208
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base values for n = 0..4 */
    static const seq_t base5[5] = {
        (seq_t)0,          /* a(0) */
        (seq_t)2,          /* a(1) */
        (seq_t)10,         /* a(2) */
        (seq_t)48,         /* a(3) */
        (seq_t)208         /* a(4) */
    };

    if (n < 5) return base5[n];

    /* iterate using the recurrence, keeping the last 5 terms */
    seq_t a0 = base5[0];
    seq_t a1 = base5[1];
    seq_t a2 = base5[2];
    seq_t a3 = base5[3];
    seq_t a4 = base5[4];
    seq_t ai = 0;

    for (idx_t i = 5; i <= n; ++i) {
        ai = (seq_t)7 * a4 - (seq_t)12 * a3 - (seq_t)4 * a2
              + (seq_t)12 * a1 - (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return ai;
}

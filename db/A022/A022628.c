/*
 * A022628  Expansion of 1/((1-x)*(1-5*x)*(1-8*x)*(1-11*x)).
 * https://oeis.org/A022628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*11^(n+3) - 40*8^(n+3) + 35*5^(n+3) - 9)/2520;
 *            Recurrence: a(n) = 25*a(n-1)-207*a(n-2)+623*a(n-3)-440*a(n-4)
 *            for n>3, with a(0)=1, a(1)=25, a(2)=418, a(3)=5898
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)418;
    if (n == 3) return (seq_t)5898;

    /* Iterate using a rolling window of the last four values. */
    seq_t a0 = (seq_t)1;       /* a(n-4) */
    seq_t a1 = (seq_t)25;      /* a(n-3) */
    seq_t a2 = (seq_t)418;     /* a(n-2) */
    seq_t a3 = (seq_t)5898;    /* a(n-1) */
    seq_t an = 0;              /* a(n) */

    /* Compute terms from 4 up to n. */
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)25 * a3
             - (seq_t)207 * a2
             + (seq_t)623 * a1
             - (seq_t)440 * a0;
        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}

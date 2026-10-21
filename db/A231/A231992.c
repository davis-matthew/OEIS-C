/*
 * A231992  Number of (n+1) X (2+1) 0..1 arrays with no element having a
 *          strict majority of its horizontal and antidiagonal neighbors equal
 *          to one.
 * https://oeis.org/A231992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 12*a(n-2) - 5*a(n-3) - 19*a(n-4) + 2*a(n-5) +
 *            4*a(n-6), with a(1)=6, a(2)=32, a(3)=156, a(4)=800, a(5)=4000,
 *            a(6)=20228
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* unused, offset starts at 1 */

    if (n <= 6) {
        static const seq_t base[6] = {
            (seq_t)6,   (seq_t)32,   (seq_t)156,
            (seq_t)800, (seq_t)4000, (seq_t)20228
        };
        return base[n - 1];
    }

    /* initialise the first six values */
    seq_t a1 = (seq_t)6,      /* a(n-6) */
          a2 = (seq_t)32,     /* a(n-5) */
          a3 = (seq_t)156,    /* a(n-4) */
          a4 = (seq_t)800,    /* a(n-3) */
          a5 = (seq_t)4000,   /* a(n-2) */
          a6 = (seq_t)20228;  /* a(n-1) */

    for (idx_t i = 7; i <= n; ++i) {
        /* compute a(i) from the last six values */
        seq_t sum1 = (seq_t)3 * a6 + (seq_t)12 * a5 +
                     (seq_t)2 * a2 + (seq_t)4 * a1;
        seq_t sum2 = (seq_t)5 * a4 + (seq_t)19 * a3;
        seq_t val  = sum1 - sum2;             /* a(i) = sum1 - sum2 */

        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = val;
    }

    return a6;
}

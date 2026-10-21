/*
 * A211766  Number of -3..3 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          three, four, five, six, seven or eight distinct values for every
 *          i,j,k<=n.
 * https://oeis.org/A211766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A211766 – number of -3..3 arrays with the stated property
 * Recurrence (empirical):
 *   a(n) = 24*a(n-1) - 206*a(n-2) + 684*a(n-3) - 251*a(n-4)
 *            - 1740*a(n-5) - 1210*a(n-6) - 300*a(n-7) - 24*a(n-8)
 * initial values: a(1)=48, a(2)=330, a(3)=2264, a(4)=15512,
 *                 a(5)=106128, a(6)=725040, a(7)=4946132, a(8)=33693740
 */
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0)                /* sequence starts at 1; return 0 for safety */
        return (seq_t)0;

    /* initial 8 terms */
    static const seq_t init[8] = {
        (seq_t)48,
        (seq_t)330,
        (seq_t)2264,
        (seq_t)15512,
        (seq_t)106128,
        (seq_t)725040,
        (seq_t)4946132,
        (seq_t)33693740
    };

    if (n <= 8)
        return init[n - 1];

    /* circular buffer holding the last 8 computed terms */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    /* compute terms 9 .. n using the recurrence */
    for (idx_t t = 9; t <= n; ++t) {
        /* indices of the previous 8 terms (mod 8) */
        idx_t i1 = (t - 2) % 8; /* a(t-1) */
        idx_t i2 = (t - 3) % 8; /* a(t-2) */
        idx_t i3 = (t - 4) % 8; /* a(t-3) */
        idx_t i4 = (t - 5) % 8; /* a(t-4) */
        idx_t i5 = (t - 6) % 8; /* a(t-5) */
        idx_t i6 = (t - 7) % 8; /* a(t-6) */
        idx_t i7 = (t - 8) % 8; /* a(t-7) */
        idx_t i8 = (t - 9) % 8; /* a(t-8) */

        seq_t v =
            buf[i1] * (seq_t)24
          - buf[i2] * (seq_t)206
          + buf[i3] * (seq_t)684
          - buf[i4] * (seq_t)251
          - buf[i5] * (seq_t)1740
          - buf[i6] * (seq_t)1210
          - buf[i7] * (seq_t)300
          - buf[i8] * (seq_t)24;

        buf[(t - 1) % 8] = v;   /* store a(t) */
    }

    return buf[(n - 1) % 8];
}

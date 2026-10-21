/*
 * A235299  Number of (n+1) X (9+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 17*a(n-2) - 57*a(n-3) - 86*a(n-4) + 372*a(n-5)
 *            + 76*a(n-6) - 972*a(n-7) + 360*a(n-8) + 864*a(n-9) - 576*a(n-10)
 *            Initial values: a(1)=96640, a(2)=104988, a(3)=115952,
 *            a(4)=138744, a(5)=172288, a(6)=242592, a(7)=358016, a(8)=603216,
 *            a(9)=1044640, a(10)=2006448
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[10] = {
    (seq_t)96640ull,
    (seq_t)104988ull,
    (seq_t)115952ull,
    (seq_t)138744ull,
    (seq_t)172288ull,
    (seq_t)242592ull,
    (seq_t)358016ull,
    (seq_t)603216ull,
    (seq_t)1044640ull,
    (seq_t)2006448ull
};

seq_t a(idx_t n)
{
    if (n <= 10) {
        return init[(size_t)(n - 1)];
    }

    seq_t buf[10];
    for (size_t i = 0; i < 10; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 11; k <= n; ++k) {
        /* Compute a(k) using the recurrence and current buffer */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        pos += buf[9] * (seq_t)3;   /* a(k-1) */
        pos += buf[8] * (seq_t)17;  /* a(k-2) */
        pos += buf[5] * (seq_t)372; /* a(k-5) */
        pos += buf[4] * (seq_t)76;  /* a(k-6) */
        pos += buf[2] * (seq_t)360; /* a(k-8) */
        pos += buf[1] * (seq_t)864; /* a(k-9) */
        neg += buf[7] * (seq_t)57;  /* a(k-3) */
        neg += buf[6] * (seq_t)86;  /* a(k-4) */
        neg += buf[3] * (seq_t)972; /* a(k-7) */
        neg += buf[0] * (seq_t)576; /* a(k-10) */

        seq_t term = pos - neg;

        /* Shift buffer left to make room for the new term */
        for (size_t i = 0; i < 9; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[9] = term;          /* buf[9] now holds a(k) */
    }

    return buf[9];
}

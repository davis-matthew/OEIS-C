/*
 * A134438  Number of tilings of a 3 X n rectangle with n trominoes.
 * https://oeis.org/A134438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 6*a(n-3) + a(n-4) - a(n-6) initial
 *            values: a(0)=1, a(1)=1, a(2)=3, a(3)=10, a(4)=23, a(5)=62,
 *            a(6)=170
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base_vals[7] = {
    (seq_t)1,  /* a(0) */
    (seq_t)1,  /* a(1) */
    (seq_t)3,  /* a(2) */
    (seq_t)10, /* a(3) */
    (seq_t)23, /* a(4) */
    (seq_t)62, /* a(5) */
    (seq_t)170 /* a(6) */
};

seq_t a(idx_t n)
{
    if (n < 7)
        return base_vals[n];

    /* Sliding window holding a(i-7) ... a(i-1). Initially holds a(0)...a(6). */
    seq_t w[7] = { base_vals[0], base_vals[1], base_vals[2],
                   base_vals[3], base_vals[4], base_vals[5],
                   base_vals[6] };

    for (idx_t i = 7; i <= n; ++i) {
        seq_t val = w[6]                    /* a(i-1) */
                  + (seq_t)2 * w[5]          /* 2*a(i-2) */
                  + (seq_t)6 * w[4]          /* 6*a(i-3) */
                  + w[3]                    /* a(i-4) */
                  - w[1];                  /* -a(i-6) */

        /* Shift window left by one */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = w[5];
        w[5] = w[6];
        w[6] = val;
    }

    return w[6];
}

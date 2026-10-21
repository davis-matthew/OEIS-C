/*
 * A200784  Number of 0..7 arrays x(0..n+1) of n+2 elements without any two
 *          consecutive increases.
 * https://oeis.org/A200784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 56*a(n-3) + 70*a(n-4) - 28*a(n-6) + 8*a(n-7)
 *            with initial values a(1)=456, a(2)=3270, a(3)=23136,
 *            a(4)=164004, a(5)=1160616, a(6)=8216484, a(7)=58154912
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base cases */
    static const seq_t base[8] = {
        (seq_t)0,        /* unused for n=0 */
        (seq_t)456,      /* a(1) */
        (seq_t)3270,     /* a(2) */
        (seq_t)23136,    /* a(3) */
        (seq_t)164004,   /* a(4) */
        (seq_t)1160616,  /* a(5) */
        (seq_t)8216484,  /* a(6) */
        (seq_t)58154912  /* a(7) */
    };

    if (n <= 7) {
        if (n == 0) return (seq_t)0;   /* safety for possible n=0 calls */
        return base[(idx_t)n];
    }

    /* Circular buffer holding the last seven values:
       buf[0] = a(n-7), buf[1] = a(n-6), ..., buf[6] = a(n-1) */
    seq_t buf[7];
    buf[0] = base[1]; /* a(1) */
    buf[1] = base[2]; /* a(2) */
    buf[2] = base[3]; /* a(3) */
    buf[3] = base[4]; /* a(4) */
    buf[4] = base[5]; /* a(5) */
    buf[5] = base[6]; /* a(6) */
    buf[6] = base[7]; /* a(7) */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = (seq_t)8 * buf[6]
                    - (seq_t)56 * buf[4]
                    + (seq_t)70 * buf[3]
                    - (seq_t)28 * buf[1]
                    + (seq_t)8  * buf[0];

        /* Shift buffer left by one position */
        for (int j = 0; j < 6; ++j)
            buf[j] = buf[j + 1];
        buf[6] = next;
    }

    return buf[6];
}

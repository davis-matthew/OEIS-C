/*
 * A188781  Number of 6-turn bishop's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A188781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 12*a(n-3) + 17*a(n-4) + 8*a(n-5) -
 *            28*a(n-6) + 8*a(n-7) + 17*a(n-8) - 12*a(n-9) - 2*a(n-10) +
 *            4*a(n-11) - a(n-12) Initial values: a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=840, a(5)=15824, a(6)=112680, a(7)=516160, a(8)=1778608,
 *            a(9)=5082912, a(10)=12622640, a(11)=28225472, a(12)=...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 20, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[13] = {
    0,          /* [0] unused */
    0,          /* a(1) */
    0,          /* a(2) */
    0,          /* a(3) */
    840,        /* a(4) */
    15824,      /* a(5) */
    112680,     /* a(6) */
    516160,     /* a(7) */
    1778608,    /* a(8) */
    5082912,    /* a(9) */
    12622640,   /* a(10) */
    28225472,   /* a(11) */
    58013112    /* a(12) */
};

/* Main function */
seq_t a(idx_t n)
{
    if (n <= 12)
        return init_vals[n];

    /* buffer to hold the last 12 terms; index 0 = a(n-12), 11 = a(n-1) */
    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i)
        buf[i] = init_vals[i + 1];   /* init_vals[1]..[12] */

    for (idx_t cur = 13; cur <= n; ++cur) {
        /* compute next term using buffered values */
        seq_t pos = 0, neg = 0;

        pos += (seq_t)4  * buf[11];
        neg += (seq_t)2  * buf[10];
        pos += (seq_t)17 * buf[8];
        neg += (seq_t)12 * buf[9];
        pos += (seq_t)8  * buf[7];
        neg += (seq_t)28 * buf[6];
        pos += (seq_t)8  * buf[5];
        pos += (seq_t)17 * buf[4];
        neg += (seq_t)12 * buf[3];
        neg += (seq_t)2  * buf[2];
        pos += (seq_t)4  * buf[1];
        neg += buf[0];

        seq_t next = pos - neg;

        /* shift buffer left, insert next at the end */
        for (idx_t j = 0; j < 11; ++j)
            buf[j] = buf[j + 1];
        buf[11] = next;
    }

    return buf[11];
}

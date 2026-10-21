/*
 * A218593  Hilltop maps: number of 2Xn binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal or
 *          antidiagonal neighbor in a random 0..2 2Xn array.
 * https://oeis.org/A218593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +3*a(n-2) +3*a(n-3) +a(n-4) +5*a(n-5) +2*a(n-6)
 *            +2*a(n-7) +a(n-8) +a(n-9) -a(n-10) -a(n-11) -a(n-12) -a(n-13)
 *            Initial values a(1)…a(14) taken from OEIS
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial terms a(1)..a(14) */
    static const seq_t init[15] = {
        0ULL,                 /* unused index 0 */
        1ULL,                 /* a(1) */
        13ULL,                /* a(2) */
        57ULL,                /* a(3) */
        225ULL,               /* a(4) */
        895ULL,               /* a(5) */
        3565ULL,              /* a(6) */
        14189ULL,             /* a(7) */
        56489ULL,             /* a(8) */
        224891ULL,            /* a(9) */
        895325ULL,            /* a(10) */
        3564433ULL,           /* a(11) */
        14190565ULL,          /* a(12) */
        56494859ULL,          /* a(13) */
        224914869ULL          /* a(14) */
    };

    if (n <= 14) {
        return init[n];
    }

    /* buffer holds the 13 values a(k-13) … a(k-1) while computing a(k) */
    seq_t buf[13];
    /* initialise buf for k = 15: a(2)…a(14) */
    for (int i = 0; i < 13; ++i) {
        buf[i] = init[i + 2];
    }

    for (idx_t k = 15; k <= n; ++k) {
        seq_t next =
            (seq_t)3 * buf[12] + (seq_t)3 * buf[11] + (seq_t)3 * buf[10] +
            buf[9] + (seq_t)5 * buf[8] + (seq_t)2 * buf[7] + (seq_t)2 * buf[6] +
            buf[5] + buf[4] - buf[3] - buf[2] - buf[1] - buf[0];

        /* shift buffer left: a(k-12)…a(k-1) become a(k-13)…a(k-2) */
        for (int i = 0; i < 12; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[12] = next;   /* a(k) now in buf[12] for next iteration */
    }

    return buf[12];
}

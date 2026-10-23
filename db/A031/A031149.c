/*
 * A031149  Numbers whose square with its last digit deleted is also a square.
 * https://oeis.org/A031149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 38*a(n-7) - a(n-14) for n > 15 Initial values: a(1)=0,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=7, a(7)=13, a(8)=16,
 *            a(9)=19, a(10)=38, a(11)=57, a(12)=136, a(13)=253, a(14)=487,
 *            a(15)=604. This implementation computes the sequence iteratively
 *            using a sliding window of 16 terms, so it works fo...
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 45, u64 n <= 88, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base[16] = {
    0,   /* dummy index 0 */
    0,   /* a(1) */
    1,   /* a(2) */
    2,   /* a(3) */
    3,   /* a(4) */
    4,   /* a(5) */
    7,   /* a(6) */
    13,  /* a(7) */
    16,  /* a(8) */
    19,  /* a(9) */
    38,  /* a(10) */
    57,  /* a(11) */
    136, /* a(12) */
    253, /* a(13) */
    487, /* a(14) */
    604  /* a(15) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n<1, guard */
    if (n <= 15) return base[(int)n];

    seq_t ring[16];
    /* initialise ring with the first 15 known terms */
    for (int i = 1; i <= 15; ++i) ring[i] = base[i];

    for (idx_t i = 16; i <= n; ++i) {
        seq_t val = 38 * ring[(i - 7) & 15] - ring[(i - 14) & 15];
        ring[i & 15] = val;
    }
    return ring[n & 15];
}

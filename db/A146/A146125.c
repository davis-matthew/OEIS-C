/*
 * A146125  Number of n X n arrays of squares of integers with every (n-1) X
 *          (n-1) subblock summing to 5 and every element equal to at least
 *          one neighbor.
 * https://oeis.org/A146125
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n>11 Initial values (offset 3): a(3)=0, a(4)=93, a(5)=2046,
 *            a(6)=8675, a(7)=24622, a(8)=56389, a(9)=112078, a(10)=201457,
 *            a(11)=335974, a(12)=528757
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values 0..12; 0..2 unused, a(3)=0 */
    static const seq_t base[13] = {
        0ULL, 0ULL, 0ULL,          /* 0,1,2 */
        0ULL,                      /* 3 */
        93ULL,                     /* 4 */
        2046ULL,                   /* 5 */
        8675ULL,                   /* 6 */
        24622ULL,                  /* 7 */
        56389ULL,                  /* 8 */
        112078ULL,                 /* 9 */
        201457ULL,                 /* 10 */
        335974ULL,                 /* 11 */
        528757ULL                  /* 12 */
    };

    if (n < 3) return 0;
    if (n <= 12) return base[n];

    /* constants for the recurrence */
    const seq_t five = 5;
    const seq_t ten  = 10;

    /* sliding window of the last five terms: a(i-5) .. a(i-1) */
    seq_t v[5];
    v[0] = base[8];   /* a(8) */
    v[1] = base[9];   /* a(9) */
    v[2] = base[10];  /* a(10) */
    v[3] = base[11];  /* a(11) */
    v[4] = base[12];  /* a(12) */

    seq_t next = 0;
    for (idx_t i = 13; i <= n; ++i) {
        next = five * v[4] - ten * v[3] + ten * v[2]
               - five * v[1] + v[0];

        /* shift window */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }
    return next;
}

/*
 * A220926  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and columns, 0..3 n X 2 array.
 * https://oeis.org/A220926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1)-36*a(n-2)+84*a(n-3)-126*a(n-4)+126*a(n-5)
 *            -84*a(n-6)+36*a(n-7)-9*a(n-8)+a(n-9) for n > 12, with initial
 *            values a(1)=4, a(2)=13, a(3)=41, a(4)=114, a(5)=260, a(6)=545,
 *            a(7)=1104, a(8)=2182, a(9)=4201, a(10)=7862, a(11)=14295,
 *            a(12)=25272. The sequence is computed using only integer...
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 56, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 12 terms (1‑based index). */
    static const seq_t init[12] = {
        (seq_t)4,   (seq_t)13,   (seq_t)41,
        (seq_t)114, (seq_t)260,  (seq_t)545,
        (seq_t)1104, (seq_t)2182, (seq_t)4201,
        (seq_t)7862, (seq_t)14295, (seq_t)25272
    };

    if (n == 0) return (seq_t)0;               /* undefined, but keep type */
    if (n <= 12) return init[n - 1];

    /* Ring buffer holding a(k-1) … a(k-9).  Index 0 holds the most recent. */
    seq_t buf[9];
    /* Initialise buffer for k = 13: buf[0] = a(12), … , buf[8] = a(4). */
    for (idx_t i = 0; i < 9; ++i)
        buf[i] = init[11 - i];

    seq_t pos, neg, value;
    for (idx_t k = 13; k <= n; ++k) {
        /* Positive terms */
        pos = (seq_t)9 * buf[0] + (seq_t)84 * buf[2]
              + (seq_t)126 * buf[4] + (seq_t)36 * buf[6] + buf[8];
        /* Negative terms */
        neg = (seq_t)36 * buf[1] + (seq_t)126 * buf[3]
              + (seq_t)84 * buf[5] + (seq_t)9 * buf[7];
        value = pos - neg;             /* final a(k) */

        if (k == n) return value;      /* last iteration – return result */

        /* Shift buffer for next step: buf[1] <- buf[0], … , buf[8] <- buf[7] */
        buf[8] = buf[7];
        buf[7] = buf[6];
        buf[6] = buf[5];
        buf[5] = buf[4];
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = value;                /* new a(k) becomes most recent */
    }
    return (seq_t)0; /* should never reach */
}

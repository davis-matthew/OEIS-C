/*
 * A211499  Number of -3..3 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having one or three distinct values for every i<=n and j<=n.
 * https://oeis.org/A211499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) -23*a(n-3) - 9*a(n-4) +62*a(n-5) * -
 *            2*a(n-6) -74*a(n-7) +10*a(n-8) +40*a(n-9) -4*a(n-10) -8*a(n-11)
 *            * with a(1)=39, a(2)=97, a(3)=207, a(4)=429, a(5)=869, *
 *            a(6)=1733, a(7)=3449, a(8)=6815, a(9)=13487, a(10)=26619,
 *            a(11)=52629
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* initial values a(1) .. a(11) */
    static const seq_t init[12] = {
        0, 39, 97, 207, 429, 869,
        1733, 3449, 6815, 13487, 26619, 52629
    };

    if (n <= 11) {
        return init[n];
    }

    /* circular buffer holding the last 11 terms */
    seq_t buf[11];
    for (idx_t i = 1; i <= 11; ++i) {
        buf[i-1] = init[i];
    }

    for (idx_t cur = 12; cur <= n; ++cur) {
        seq_t pos = 0, neg = 0;

        /* positive contributions */
        pos += (seq_t)3  * buf[10];   /* a(cur-1) */
        pos += (seq_t)6  * buf[9];    /* a(cur-2) */
        pos += (seq_t)62 * buf[6];    /* a(cur-5) */
        pos += (seq_t)10 * buf[3];    /* a(cur-8) */
        pos += (seq_t)40 * buf[2];    /* a(cur-9) */

        /* negative contributions */
        neg += (seq_t)23 * buf[8];    /* a(cur-3) */
        neg += (seq_t) 9 * buf[7];    /* a(cur-4) */
        neg += (seq_t) 2 * buf[5];    /* a(cur-6) */
        neg += (seq_t)74 * buf[4];    /* a(cur-7) */
        neg += (seq_t) 4 * buf[1];    /* a(cur-10) */
        neg += (seq_t) 8 * buf[0];    /* a(cur-11) */

        seq_t val = pos - neg;   /* all intermediate results are non‑negative */

        /* shift buffer left and insert new value */
        for (idx_t k = 0; k < 10; ++k) {
            buf[k] = buf[k+1];
        }
        buf[10] = val;
    }

    return buf[10];
}

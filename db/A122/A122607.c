/*
 * A122607  Expansion of x*(8*x^5 + 5*x^4 - x^3 - 5*x^2 - 1)/(x^6 + 3*x^5 +
 *          6*x^4 + 4*x^3 - 5*x^2 + x - 1).
 * https://oeis.org/A122607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1)-5*a(n-2)+4*a(n-3)+6*a(n-4)+3*a(n-5)+a(n-6), for
 *            n>=7 with initial values a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 29, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 6) return 1;

    /* Keep the last six values: buf[0]=a(n-6), ..., buf[5]=a(n-1) */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i) buf[i] = 1;   /* a(1)..a(6) */

    for (idx_t cur = 7; cur <= n; ++cur) {
        seq_t next = buf[5]          /* a(cur-1) */
                   - (seq_t)5 * buf[4] /* a(cur-2) */
                   + (seq_t)4 * buf[3] /* a(cur-3) */
                   + (seq_t)6 * buf[2] /* a(cur-4) */
                   + (seq_t)3 * buf[1] /* a(cur-5) */
                   + buf[0];           /* a(cur-6) */

        /* Shift buffer left: buf[0] becomes a(cur-5), ..., buf[5] becomes a(cur) */
        for (int k = 0; k < 5; ++k) buf[k] = buf[k + 1];
        buf[5] = next;
    }

    /* After loop, buf[5] holds a(n) */
    return buf[5];
}

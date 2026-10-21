/*
 * A077963  Expansion of 1/(1+x^2+2*x^3).
 * https://oeis.org/A077963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-2) - 2*a(n-3) for n ≥ 3 with initial values a(0)=1,
 *            a(1)=0, a(2)=-1
 * Range:     i8 n <= 15, i16 n <= 30, i32 n <= 64, i64 n <= 126,
 *            i128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)-1;

    /* Use a circular buffer of size 3 to store the last three terms. */
    seq_t buf[3];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)0;   /* a(1) */
    buf[2] = (seq_t)-1;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_i2 = buf[(i-2) % 3];
        seq_t a_i3 = buf[(i-3) % 3];
        seq_t new_val = -(a_i2 + (seq_t)2 * a_i3);
        buf[i % 3] = new_val;
    }
    return buf[n % 3];
}

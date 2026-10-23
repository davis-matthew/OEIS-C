/*
 * A106330  Numbers k such that k^2 = 24*j^2 + 25.
 * https://oeis.org/A106330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5, a(2)=7, a(3)=11, a(4)=25,
   a(5)=10*a(2)-a(3), a(6)=10*a(3)-a(2),
   a(n)=10*a(n-3)-a(n-6) for n≥7. */

#include <stdint.h>

/* idx_t is an unsigned integer type (uint64_t), seq_t is the sequence element type. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; undefined for n=0 */
        return 0u;

    /* Initial six values */
    seq_t k1 = 5;
    seq_t k2 = 7;
    seq_t k3 = 11;
    seq_t k4 = 25;
    seq_t k5 = (seq_t)10 * k2 - k3;   /* 10*7-11 = 59 */
    seq_t k6 = (seq_t)10 * k3 - k2;   /* 10*11-7 = 103 */

    /* Return directly if n is within the first six terms */
    if (n == 1) return k1;
    if (n == 2) return k2;
    if (n == 3) return k3;
    if (n == 4) return k4;
    if (n == 5) return k5;
    if (n == 6) return k6;

    /* For n≥7, use a circular buffer holding the last six values:
       buf[0] = a(n-6), buf[1] = a(n-5), ..., buf[5] = a(n-1)   */
    seq_t buf[6] = {k1, k2, k3, k4, k5, k6};
    seq_t val = 0; /* will hold the current a(i) */
    for (idx_t i = 7; i <= n; ++i) {
        val = (seq_t)10 * buf[3] - buf[0];  /* a(i) = 10*a(i-3) - a(i-6) */
        /* Shift buffer: drop buf[0], shift left, put new value at end */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = val;
    }
    return val;
}

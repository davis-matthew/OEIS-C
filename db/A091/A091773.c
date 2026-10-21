/*
 * A091773  G.f.: Product((1+x^i)/(1-x^i),i=1..n-1)/(1-x^n), with n = 5.
 * https://oeis.org/A091773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 56, u32 n <= 935, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 3*a(n-1)-3*a(n-2)+2*a(n-3)-3*a(n-4)+4*a(n-5)-4*a(n-6)
          +3*a(n-7)-2*a(n-8)+3*a(n-9)-3*a(n-10)+a(n-11)
   Initial values:
   a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=14, a(5)=23,
   a(6)=36, a(7)=54, a(8)=78, a(9)=110, a(10)=151 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* small initial segment */
    static const seq_t init[11] = {1,2,4,8,14,23,36,54,78,110,151};

    if (n < 11) {
        return init[(size_t)n];
    }

    /* buffer holding last 11 values:
       buf[0] = a(i-11), … , buf[10] = a(i-1)   */
    seq_t buf[11];
    for (size_t j = 0; j < 11; ++j) {
        buf[j] = init[j];
    }

    /* iteratively compute a(i) up to desired n */
    for (idx_t i = 11; i <= n; ++i) {
        /* positive part of recurrence (coefficients applied to
           a(i-1), a(i-3), a(i-5), a(i-7), a(i-9), a(i-11)) */
        seq_t pos = buf[10] * (seq_t)3
                  + buf[8] * (seq_t)2
                  + buf[6] * (seq_t)4
                  + buf[4] * (seq_t)3
                  + buf[2] * (seq_t)3
                  + buf[0];
        /* negative part (coefficients applied to
           a(i-2), a(i-4), a(i-6), a(i-8), a(i-10)) */
        seq_t neg = buf[9] * (seq_t)3
                  + buf[7] * (seq_t)3
                  + buf[5] * (seq_t)4
                  + buf[3] * (seq_t)2
                  + buf[1] * (seq_t)3;
        seq_t next = pos - neg;   /* pos > neg for all n >= 11 */

        /* shift buffer left to drop oldest value and add new one */
        for (size_t j = 0; j < 10; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[10] = next;
    }

    return buf[10];
}

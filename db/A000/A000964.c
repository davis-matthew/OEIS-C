/*
 * A000964  The convergent sequence C_n for the ternary continued fraction
 *          (3,1;2,2) of period 2.
 * https://oeis.org/A000964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-2) - 3*a(n-4) + a(n-6); a(0)=0, a(1)=0, a(2)=1,
 *            a(3)=1, a(4)=4, a(5)=8
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t w[6];
    idx_t i;
    int j;
    w[0] = (seq_t)0;
    w[1] = (seq_t)0;
    w[2] = (seq_t)1;
    w[3] = (seq_t)1;
    w[4] = (seq_t)4;
    w[5] = (seq_t)8;
    if (n < 6) {
        return w[n];
    }
    /* w[k] holds a(i-6+k) for the current i */
    for (i = 6; i <= n; i++) {
        seq_t next = (seq_t)7 * w[4] + w[0] - (seq_t)3 * w[2];
        for (j = 0; j < 5; j++) {
            w[j] = w[j + 1];
        }
        w[5] = next;
    }
    return w[5];
}

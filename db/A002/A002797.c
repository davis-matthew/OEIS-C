/*
 * A002797  Number of solutions to a linear inequality.
 * https://oeis.org/A002797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-5) - a(n-6) +
 *            a(n-7), a(0) = 3, a(1) = 2, a(2) = 5, a(3) = 9, a(4) = 17, a(5)
 *            = 27, a(6) = 40
 * Range:     u8 n <= 14, u16 n <= 224, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t w[7];
    idx_t i;
    int j;
    w[0] = 3; w[1] = 2; w[2] = 5; w[3] = 9;
    w[4] = 17; w[5] = 27; w[6] = 40;
    if (n < 7) return w[(int)n];
    for (i = 7; i <= n; i++) {
        /* w[6]=a(i-1), w[5]=a(i-2), ..., w[0]=a(i-7) */
        seq_t next = w[6] + w[5] + w[3] + w[0];
        next -= w[4];
        next -= w[2];
        next -= w[1];
        for (j = 0; j < 6; j++) w[j] = w[j + 1];
        w[6] = next;
    }
    return w[6];
}

/*
 * A005822  G.f.: x*(1-x^2)*(x^4+x^3-x^2+x+1) / (x^8-4*x^6-x^4-4*x^2+1).
 * https://oeis.org/A005822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + a(n-4) + 4*a(n-6) - a(n-8) for n >= 8, a(0..7)
 *            = 0, 1, 1, 2, 4, 11, 16, 49
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t w[8];
    seq_t next;
    idx_t i;
    int j;

    w[0] = 0; w[1] = 1; w[2] = 1; w[3] = 2;
    w[4] = 4; w[5] = 11; w[6] = 16; w[7] = 49;

    if (n < 8) {
        return w[n];
    }

    /* w[j] holds a(i-8+j) */
    for (i = 8; i <= n; i++) {
        next = 4 * w[6] + w[4] + 4 * w[2];
        next -= w[0];
        for (j = 0; j < 7; j++) {
            w[j] = w[j + 1];
        }
        w[7] = next;
    }
    return w[7];
}

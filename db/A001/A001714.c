/*
 * A001714  Generalized Stirling numbers, [n+7,7]_3.
 * https://oeis.org/A001714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^4] Product_{r=0..n+3} (x + 3 + r)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t c[5];
    int j;
    idx_t v;
    c[0] = 1;
    for (j = 1; j < 5; j++) c[j] = 0;
    for (v = 3; v <= n + 6; v++) {
        for (j = 4; j >= 1; j--) {
            c[j] = c[j - 1] + (seq_t)v * c[j];
        }
        c[0] = c[0] * (seq_t)v;
    }
    return c[4];
}

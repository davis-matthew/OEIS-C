/*
 * A001875  Convolved Fibonacci numbers.
 * https://oeis.org/A001875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. (1 - x - x^2)^(-7); computed by applying 1/(1-x-x^2) seven
 *            times: c_j(i) = c_{j-1}(i) + c_j(i-1) + c_j(i-2), c_0(i) =
 *            [i=0], c_j(-1) = c_j(-2) = 0, and a(n) = c_7(n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 22, u64 n <= 58, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t p1[8], p2[8];
    seq_t cur = 0;
    int j;
    idx_t i;
    for (j = 0; j < 8; j++) {
        p1[j] = 0;
        p2[j] = 0;
    }
    for (i = 0; i <= n; i++) {
        cur = (i == 0) ? 1 : 0;
        for (j = 1; j <= 7; j++) {
            seq_t v = cur + p1[j] + p2[j];
            p2[j] = p1[j];
            p1[j] = v;
            cur = v;
        }
    }
    return cur;
}

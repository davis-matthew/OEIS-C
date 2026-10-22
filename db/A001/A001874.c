/*
 * A001874  Convolved Fibonacci numbers.
 * https://oeis.org/A001874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: (1 - x - x^2)^(-6); computed as six successive divisions
 *            by (1 - x - x^2): L_0(i) = [i==0], L_j(i) = L_{j-1}(i) +
 *            L_j(i-1) + L_j(i-2), L_j(-1) = L_j(-2) = 0, and a(n) = L_6(n)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 24, u64 n <= 62, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t p1[7], p2[7], cur[7];
    int j;
    idx_t i;
    for (j = 0; j < 7; j++) {
        p1[j] = 0;
        p2[j] = 0;
        cur[j] = 0;
    }
    for (i = 0; i <= n; i++) {
        cur[0] = (i == 0) ? 1 : 0;
        for (j = 1; j < 7; j++) {
            cur[j] = cur[j - 1] + p1[j] + p2[j];
        }
        for (j = 0; j < 7; j++) {
            p2[j] = p1[j];
            p1[j] = cur[j];
        }
    }
    return cur[6];
}

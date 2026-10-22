/*
 * A005715  Coefficient of x^7 in expansion of (1+x+x^2)^n.
 * https://oeis.org/A005715
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^7] (1+x+x^2)^n, computed by repeated multiplication
 *            truncated to degree 7 (additions only)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 77, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t c[8];
    int k;
    idx_t i;
    for (k = 0; k < 8; k++) c[k] = 0;
    c[0] = 1;
    for (i = 0; i < n; i++) {
        for (k = 7; k >= 2; k--) c[k] = c[k] + c[k - 1] + c[k - 2];
        c[1] = c[1] + c[0];
    }
    return c[7];
}

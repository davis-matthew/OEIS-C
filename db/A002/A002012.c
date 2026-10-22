/*
 * A002012  Number of rooted planar maps with three trivalent vertices and a
 *          root vertex of degree 2*n - 1.
 * https://oeis.org/A002012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n+3)*(2*(n+1))! / (3*n!*(n+1)!) =
 *            2*(n+3)*(n+1)*binomial(2n+2,n+1)/3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t m = n + 1;
    seq_t c = 1;
    idx_t i;
    for (i = 1; i <= m; i++) {
        c = c * (seq_t)(m + i);
        c = c / (seq_t)i;
    }
    uint64_t x = 2 * (uint64_t)(n + 3) * (uint64_t)(n + 1);
    if (x % 3 == 0) {
        x /= 3;
    } else {
        c = c / 3;
    }
    return c * (seq_t)x;
}

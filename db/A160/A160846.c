/*
 * A160846  Number of lines through at least 2 points of a 6 X n grid of
 *          points.
 * https://oeis.org/A160846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(f(6,n,1)-f(6,n,2)) where f(m,n,k) =
 *            Σ_{(x,y)≠(0,0), gcd(x,y)=1} (n-|kx|)*(m-|ky|) 1 ≤ |x| < n/k, 1 ≤
 *            |y| < m/k. Here m = 6
 * Range:     u8 n <= 3, u16 n <= 88, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    const idx_t m = 6;
    const idx_t maxY1 = 5;          /* (m-1)/1 */
    const idx_t maxY2 = 2;          /* (m-1)/2 */
    const idx_t maxX1 = n - 1;      /* (n-1)/1 */
    const idx_t maxX2 = (n - 1) / 2;/* (n-1)/2 */

    seq_t f1 = 0;
    seq_t f2 = 0;

    /* k = 1 */
    for (idx_t x = 0; x <= maxX1; ++x) {
        for (idx_t y = 0; y <= maxY1; ++y) {
            if (x == 0 && y == 0) continue;
            if (gcd_idx(x, y) != 1) continue;
            seq_t w = ((x == 0) ? 1 : 2) * ((y == 0) ? 1 : 2);
            seq_t term = (seq_t)(n - x) * (seq_t)(m - y);
            f1 += term * w;
        }
    }

    /* k = 2 */
    for (idx_t x = 0; x <= maxX2; ++x) {
        for (idx_t y = 0; y <= maxY2; ++y) {
            if (x == 0 && y == 0) continue;
            if (gcd_idx(x, y) != 1) continue;
            seq_t w = ((x == 0) ? 1 : 2) * ((y == 0) ? 1 : 2);
            seq_t term = (seq_t)(n - 2 * x) * (seq_t)(m - 2 * y);
            f2 += term * w;
        }
    }

    return (f1 - f2) / 2;
}

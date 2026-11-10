/*
 * A227835  3^a(n) is the highest power of 3 dividing A000930(n).
 * https://oeis.org/A227835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3-adic valuation of A000930(n), where A000930(n) =
 *            A000930(n-1) + A000930(n-3), A000930(0) = A000930(1) =
 *            A000930(2) = 1; computed modulo 3^80
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 129 terms through n = 5000 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 a227835_u128;

seq_t a(idx_t n) {
    a227835_u128 M = 1;
    for (int i = 0; i < 80; i++) M *= 3;
    a227835_u128 x = 1, y = 1, z = 1; /* A000930(i-2), (i-1), (i) */
    for (idx_t i = 3; i <= n; i++) {
        a227835_u128 t = z + x;
        if (t >= M) t -= M;
        x = y; y = z; z = t;
    }
    a227835_u128 r = (n <= 2) ? 1 : z;
    if (r == 0) return (seq_t)80;
    int v = 0;
    while (r % 3 == 0) { r /= 3; v++; }
    return (seq_t)v;
}

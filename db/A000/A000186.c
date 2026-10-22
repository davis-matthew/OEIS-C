/*
 * A000186  Number of 3 X n Latin rectangles in which the first row is in
 *          order.
 * https://oeis.org/A000186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n(n-1)*a(n-1) + n^2(n-1)*a(n-2) + 3n(n-1)(n-2)*a(n-3) +
 *            2n(n-1)(n-2)(n-3)*a(n-4) - (n-1)*2^n, a(0) = 1, a(k) = 0 for k <
 *            0 (derived from the e.g.f. exp(2x) Sum_{k>=0} k!
 *            x^k/(1+x)^(3k+3): x^2(1+x)A' = (1-2x^2-3x^3-2x^4)A -
 *            (1-2x)exp(2x))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x1 = 1, x2 = 0, x3 = 0, x4 = 0; /* a(i-1), a(i-2), a(i-3), a(i-4) */
    seq_t p = 1;                           /* 2^i */
    idx_t i;
    if (n == 0)
        return x1;
    for (i = 1; i <= n; i++) {
        uint64_t c1 = (uint64_t)i * (i - 1);
        uint64_t c2 = (uint64_t)i * i * (i - 1);
        uint64_t c3 = 3 * (uint64_t)i * (i - 1) * (i - 2);
        uint64_t c4 = 2 * (uint64_t)i * (i - 1) * (i - 2) * (i - 3);
        seq_t cur;
        p = p + p;
        cur = (seq_t)c1 * x1;
        cur += (seq_t)c2 * x2;
        cur += (seq_t)c3 * x3;
        cur += (seq_t)c4 * x4;
        cur -= (seq_t)(i - 1) * p;
        x4 = x3;
        x3 = x2;
        x2 = x1;
        x1 = cur;
    }
    return x1;
}

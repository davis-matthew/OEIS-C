/*
 * A004474  Nim product 9 * n.
 * https://oeis.org/A004474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(9,n), the nim product 9 (x) n, computed by
 *            recursive (Karatsuba-style) nim multiplication on 2^k-bit halves
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t nim_mul(uint64_t x, uint64_t y, int L)
{
    int h;
    uint64_t mask, x1, x0, y1, y0, p11, p00, pm, high, low;
    if (L == 1)
        return x & y;
    h = L / 2;
    mask = (((uint64_t)1) << h) - 1;
    x1 = x >> h; x0 = x & mask;
    y1 = y >> h; y0 = y & mask;
    p11 = nim_mul(x1, y1, h);
    p00 = nim_mul(x0, y0, h);
    pm = nim_mul(x1 ^ x0, y1 ^ y0, h);
    high = pm ^ p00;
    low = p00 ^ nim_mul(p11, ((uint64_t)1) << (h - 1), h);
    return (high << h) | low;
}

seq_t a(idx_t n)
{
    uint64_t r = nim_mul((uint64_t)9, (uint64_t)n, 64);
    seq_t res = 0;
    int i;
    for (i = 63; i >= 0; i--) {
        res = res * 2;
        if ((r >> i) & 1)
            res += 1;
    }
    return res;
}

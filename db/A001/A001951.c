/*
 * A001951  A Beatty sequence: a(n) = floor(n*sqrt(2)).
 * https://oeis.org/A001951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor(n*sqrt(2)) = A000196(2*n^2), the integer square
 *            root of 2*n^2
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9974 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

static seq_t a001951_isqrt_seq(seq_t N, seq_t start)
{
    seq_t x = start;
    if (x == 0)
        return x;
    for (;;) {
        seq_t y = (x + N / x) / 2;
        if (y >= x)
            break;
        x = y;
    }
    while (x * x > N)
        x--;
    while ((x + 1) * (x + 1) <= N)
        x++;
    return x;
}

seq_t a(idx_t n)
{
    if (n < ((idx_t)1 << 63)) {
        unsigned __int128 v = (unsigned __int128)n * (unsigned __int128)n * 2;
        uint64_t r = (uint64_t)isqrt_u128(v);
        return (seq_t)r;
    } else {
        seq_t sn = (seq_t)n;
        return a001951_isqrt_seq(sn * sn * 2, sn * 2);
    }
}

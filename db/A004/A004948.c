/*
 * A004948  a(n) = round(n*phi^13), where phi is the golden ratio, A001622.
 * https://oeis.org/A004948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^13) = floor((521*n + 1 +
 *            floor(233*n*sqrt(5)))/2), since phi^13 = (521 + 233*sqrt(5))/2
 *            and floor(233*n*sqrt(5)) = isqrt(271445*n^2)
 * Range:     u8 n <= 0, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static unsigned __int128 isqrt_u128(unsigned __int128 N) {
    unsigned __int128 x, y;
    int bits = 0;
    if (N < 2) return N;
    {
        unsigned __int128 t = N;
        while (t) { bits++; t >>= 1; }
    }
    x = (unsigned __int128)1 << ((bits + 1) / 2 + 1);
    y = (x + N / x) / 2;
    while (y < x) {
        x = y;
        y = (x + N / x) / 2;
    }
    return x;
}

static seq_t isqrt_seq(seq_t N) {
    seq_t x, y;
    if (N < 2) return N;
    x = N;
    y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + N / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n < ((idx_t)1 << 50)) {
        unsigned __int128 m = (unsigned __int128)n;
        unsigned __int128 s = isqrt_u128((unsigned __int128)271445 * m * m);
        return u128_to_seq((521 * m + 1 + s) / 2);
    } else {
        seq_t m = (seq_t)n;
        seq_t s = isqrt_seq(m * m * 271445);
        return (521 * m + 1 + s) / 2;
    }
}

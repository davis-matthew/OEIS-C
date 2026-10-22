/*
 * A004967  a(n) = ceiling(n*phi^12), where phi is the golden ratio, A001622.
 * https://oeis.org/A004967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^12) = 161*n + ceiling(72*n*sqrt(5)) = 161*n
 *            + floor(sqrt(25920*n^2)) + 1 for n >= 1, a(0) = 0 (since phi^12
 *            = 161 + 72*sqrt(5))
 * Range:     u8 n <= 0, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

static unsigned __int128 isqrt_u128(unsigned __int128 m) {
    unsigned __int128 x, y, t;
    int bits = 0;
    if (m < 2) return m;
    t = m;
    while (t) { bits++; t >>= 1; }
    x = (unsigned __int128)1 << ((bits + 1) / 2 + 1);
    for (;;) {
        y = (x + m / x) >> 1;
        if (y >= x) break;
        x = y;
    }
    return x;
}

static seq_t isqrt_seq(seq_t m) {
    seq_t x, y;
    if (m < 2) return m;
    x = m;
    for (;;) {
        y = (x + m / x) / 2;
        if (y >= x) break;
        x = y;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n < ((idx_t)1 << 56)) {
        unsigned __int128 nn = (unsigned __int128)n;
        unsigned __int128 r = isqrt_u128((unsigned __int128)25920 * nn * nn);
        return u128_to_seq((unsigned __int128)161 * nn + r + 1);
    } else {
        seq_t s = (seq_t)n;
        return s * 161 + isqrt_seq(s * s * 25920) + 1;
    }
}

/*
 * A004924  a(n) = floor(n*phi^9), where phi is the golden ratio, A001622.
 * https://oeis.org/A004924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi^9) = 38*n + floor(17*n*sqrt(5)) = 38*n +
 *            isqrt(1445*n^2), since phi^9 = 38 + 17*sqrt(5)
 * Range:     u8 n <= 3, u16 n <= 862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

static unsigned __int128 isqrt_u128(unsigned __int128 m) {
    unsigned __int128 r = 0, bit = (unsigned __int128)1 << 126;
    while (bit > m) bit >>= 2;
    while (bit != 0) {
        if (m >= r + bit) {
            m -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

static seq_t isqrt_seq(seq_t m) {
    seq_t x, y;
    if (m < 2) return m;
    x = m;
    y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + m / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n < ((idx_t)1 << 50)) {
        unsigned __int128 nn = (unsigned __int128)n;
        unsigned __int128 v = 38 * nn + isqrt_u128(1445 * nn * nn);
        return u128_to_seq(v);
    } else {
        seq_t x = (seq_t)n;
        return 38 * x + isqrt_seq(1445 * x * x);
    }
}

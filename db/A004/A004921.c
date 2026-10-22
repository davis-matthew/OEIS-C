/*
 * A004921  a(n) = floor(n*phi^6), phi = golden ratio, A001622.
 * https://oeis.org/A004921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((9 + 4*sqrt(5))*n) = 9*n + floor(sqrt(80*n^2))
 * Range:     u8 n <= 14, u16 n <= 3652, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_u128, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/isqrt_u128.h"

static seq_t isqrt_seq(seq_t m) {
    if (m < 2) return m;
    seq_t x = m;
    seq_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + m / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n < ((idx_t)1 << 60)) {
        unsigned __int128 nn = (unsigned __int128)n;
        unsigned __int128 r = isqrt_u128(nn * nn * 80);
        return u128_to_seq(nn * 9 + r);
    }
    seq_t s = (seq_t)n;
    return s * 9 + isqrt_seq(s * s * 80);
}

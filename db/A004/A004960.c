/*
 * A004960  a(n) = ceiling(n*phi^5), where phi is the golden ratio, A001622.
 * https://oeis.org/A004960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^5) = floor((11*n + floor(sqrt(125*n^2)))/2)
 *            + 1 for n >= 1, a(0) = 0
 * Range:     u8 n <= 22, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

static unsigned __int128 isqrt_u128(unsigned __int128 v) {
    if (v < 2) return v;
    unsigned __int128 x = v, y;
    int bits = 0;
    unsigned __int128 t = v;
    while (t) { bits++; t >>= 1; }
    x = (unsigned __int128)1 << ((bits + 1) / 2);
    for (;;) {
        y = (x + v / x) >> 1;
        if (y >= x) break;
        x = y;
    }
    return x;
}

static seq_t isqrt_seq(seq_t v) {
    if (v < 2) return v;
    seq_t x = v, y;
    for (;;) {
        y = (x + v / x) / 2;
        if (y >= x) break;
        x = y;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n < ((idx_t)1 << 60)) {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 m = isqrt_u128(N * N * 125);
        unsigned __int128 k = N * 11 + m;
        return u128_to_seq(k / 2 + 1);
    }
    seq_t N = (seq_t)n;
    seq_t m = isqrt_seq(N * N * 125);
    seq_t k = N * 11 + m;
    return k / 2 + 1;
}

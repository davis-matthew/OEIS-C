/*
 * A004975  a(n) = ceiling(n*phi^20), where phi is the golden ratio, A001622.
 * https://oeis.org/A004975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^20) = ceiling((15127*n +
 *            6765*n*sqrt(5))/2); with s = floor(sqrt(5*(6765*n)^2)), a(n) =
 *            floor((15127*n + s)/2) + 1 for n >= 1, a(0) = 0
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

static unsigned __int128 isqrt_u128(unsigned __int128 N, unsigned __int128 start) {
    unsigned __int128 x = start, y;
    if (x == 0) return 0;
    y = (x + N / x) / 2;
    while (y < x) {
        x = y;
        y = (x + N / x) / 2;
    }
    return x;
}

static seq_t isqrt_seq(seq_t N, seq_t start) {
    seq_t x = start, y;
    if (x == 0) return x;
    y = (x + N / x) / 2;
    while (y < x) {
        x = y;
        y = (x + N / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (seq_is_big()) {
        seq_t k = (seq_t)n * 6765;
        seq_t s = isqrt_seq(k * k * 5, k * 3);
        seq_t m = (seq_t)n * 15127 + s;
        return m / 2 + 1;
    } else {
        unsigned __int128 k = (unsigned __int128)n * 6765u;
        unsigned __int128 s = isqrt_u128(k * k * 5u, k * 3u);
        unsigned __int128 m = (unsigned __int128)n * 15127u + s;
        return u128_to_seq(m / 2 + 1);
    }
}

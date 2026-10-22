/*
 * A004970  a(n) = ceiling(n*phi^15), where phi is the golden ratio, A001622.
 * https://oeis.org/A004970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^15) = 682*n + ceiling(305*n*sqrt(5)) =
 *            682*n + floor(sqrt(465125*n^2)) + 1 for n >= 1, a(0) = 0
 * Range:     u8 n <= 0, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    unsigned __int128 m = (unsigned __int128)n;
    unsigned __int128 s = isqrt_u128((unsigned __int128)465125 * m * m);
    unsigned __int128 v = (unsigned __int128)682 * m + s + 1;
    return u128_to_seq(v);
}

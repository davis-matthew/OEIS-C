/*
 * A004957  a(n) = ceiling(n*phi^2), where phi is the golden ratio, A001622.
 * https://oeis.org/A004957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 + floor(n*phi) = n + 1 + floor((n +
 *            floor(sqrt(5*n^2)))/2) for n >= 1, a(0) = 0
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 res = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    unsigned __int128 m = (unsigned __int128)n;
    unsigned __int128 s = isqrt_u128(5 * m * m);
    unsigned __int128 fl = (m + s) / 2;   /* floor(n*phi) */
    return u128_to_seq(m + 1 + fl);
}

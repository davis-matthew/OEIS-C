/*
 * A001960  a(n) = floor((n+2/3)*(5+sqrt(13))/2); v-pile positions in the
 *          3-Wythoff game.
 * https://oeis.org/A001960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+2/3)*(5+sqrt(13))/2) = floor((15n+10 +
 *            floor(sqrt(13*(3n+2)^2)))/6)
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001960_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001960_isqrt128.h"

seq_t a(idx_t n) {
    unsigned __int128 A = (unsigned __int128)3 * n + 2;
    unsigned __int128 X = (unsigned __int128)15 * n + 10;
    uint64_t s = a001960_isqrt128(13 * A * A);
    unsigned __int128 r = (X + s) / 6;
    return (seq_t)(uint64_t)r;
}

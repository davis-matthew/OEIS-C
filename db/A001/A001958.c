/*
 * A001958  v-pile numbers of the 3-Wythoff game with i=1.
 * https://oeis.org/A001958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (n+1/3) * (5+sqrt(13)) /2) = floor( (5*m +
 *            floor(sqrt(13*m^2))) / 6 ) with m = 3n+1
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001958_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001958_isqrt128.h"

seq_t a(idx_t n) {
    unsigned __int128 m = (unsigned __int128)n * 3 + 1;
    unsigned __int128 s = a001958_isqrt128(m * m * 13);
    unsigned __int128 r = (m * 5 + s) / 6;
    return (seq_t)(uint64_t)r;
}

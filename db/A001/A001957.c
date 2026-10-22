/*
 * A001957  u-pile positions in the 3-Wythoff game with i=1.
 * https://oeis.org/A001957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1/3)*(sqrt(13)-1)/2) = floor((floor(m*sqrt(13))
 *            - m)/6) with m = 3n+1, where floor(m*sqrt(13)) = isqrt(13*m^2)
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a1957_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1957_isqrt128.h"

seq_t a(idx_t n) {
    unsigned __int128 m = (unsigned __int128)3 * n + 1;
    unsigned __int128 s = a1957_isqrt128(13 * m * m);
    uint64_t k = (uint64_t)((s - m) / 6);
    return (seq_t)k;
}

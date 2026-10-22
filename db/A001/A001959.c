/*
 * A001959  u-pile numbers for the 3-Wythoff game with i=2.
 * https://oeis.org/A001959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (n+2/3)*(sqrt(13)-1)/2 ) = floor(
 *            (isqrt(13*(3n+2)^2) - (3n+2)) / 6 )
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001959_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001959_isqrt.h"

seq_t a(idx_t n) {
    unsigned __int128 m = (unsigned __int128)3 * n + 2;
    unsigned __int128 s = a001959_isqrt(13 * m * m); /* floor(m*sqrt(13)) */
    uint64_t r = (uint64_t)((s - m) / 6);
    return (seq_t)r;
}

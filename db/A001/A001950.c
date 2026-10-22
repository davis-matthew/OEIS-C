/*
 * A001950  Upper Wythoff sequence (a Beatty sequence): a(n) = floor(n*phi^2),
 *          where phi = (1+sqrt(5))/2.
 * https://oeis.org/A001950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n + floor(n*phi) = n + floor((n + sqrt(5*n^2))/2)
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001950_isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001950_isqrt_u128.h"

seq_t a(idx_t n) {
    unsigned __int128 m = (unsigned __int128)n;
    unsigned __int128 s = a001950_isqrt_u128(5 * m * m);
    uint64_t lower = (uint64_t)((m + s) / 2);
    return (seq_t)n + (seq_t)lower;
}

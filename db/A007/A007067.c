/*
 * A007067  Nearest integer to n*phi where phi = (1+sqrt(5))/2.
 * https://oeis.org/A007067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1 + floor(sqrt(5*n^2)))/2), i.e. round(n*phi)
 *            = floor((n + 1 + n*sqrt(5))/2)
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a007067_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a007067_isqrt128.h"

seq_t a(idx_t n) {
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 s = a007067_isqrt128(5 * nn * nn);
    unsigned __int128 r = (nn + 1 + s) / 2;
    return (seq_t)(uint64_t)r;
}

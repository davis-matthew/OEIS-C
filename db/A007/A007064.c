/*
 * A007064  Numbers not of form "nearest integer to n*tau", tau =
 *          (1+sqrt(5))/2.
 * https://oeis.org/A007064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n*(1+tau) - tau/2) = floor((6n - 1 +
 *            (2n-1)*sqrt(5))/4) = floor((6n - 1 + floor(sqrt(5*(2n-1)^2)))/4)
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a007064_isqrt128, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a007064_isqrt128.h"

seq_t a(idx_t n) {
    unsigned __int128 k = (unsigned __int128)2 * n - 1;
    unsigned __int128 s = a007064_isqrt128(k * k * 5); /* floor(k*sqrt(5)) */
    unsigned __int128 v = ((unsigned __int128)6 * n - 1 + s) / 4;
    return u128_to_seq(v);
}

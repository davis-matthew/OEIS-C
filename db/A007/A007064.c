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
 * Validated: standard, pass, 68 terms through n = 993 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_u128.h"

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

seq_t a(idx_t n) {
    unsigned __int128 k = (unsigned __int128)2 * n - 1;
    unsigned __int128 s = isqrt_u128(k * k * 5); /* floor(k*sqrt(5)) */
    unsigned __int128 v = ((unsigned __int128)6 * n - 1 + s) / 4;
    return u128_to_seq(v);
}

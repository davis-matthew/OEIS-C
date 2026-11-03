/*
 * A004933  a(n) = floor(n*phi^18), where phi is the golden ratio, A001622.
 * https://oeis.org/A004933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*phi^18) = 2889*n + floor(1292*n*sqrt(5)) = 2889*n
 *            + isqrt(5*(1292*n)^2), since phi^18 = 2889 + 1292*sqrt(5)
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 43 terms through n = 9545 (2026-11-01)
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
    unsigned __int128 m = (unsigned __int128)n * 1292u;
    unsigned __int128 v = (unsigned __int128)n * 2889u + isqrt_u128(5u * m * m);
    return u128_to_seq(v);
}

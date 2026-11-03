/*
 * A007066  a(n) = 1 + ceiling((n-1)*phi^2), phi = (1+sqrt(5))/2.
 * https://oeis.org/A007066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 1; a(n) = floor((n-1)*phi^2) + 2 for n >= 2, where
 *            floor(m*phi^2) = floor(m*phi) + m and floor(m*phi) = floor((m +
 *            isqrt(5*m^2))/2)
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8586 (2026-11-01)
 * Helpers:   isqrt_seq, isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_seq.h"
#include "../utils/isqrt_u128.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

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
    uint64_t m;
    if (n <= 1) return (seq_t)1;
    m = (uint64_t)(n - 1);
    if (seq_is_big()) {
        seq_t M = (seq_t)m;
        seq_t N = M * M * 5;
        seq_t r = isqrt_seq(N);
        seq_t f = (M + r) / 2;
        return f + M + 2;
    } else {
        unsigned __int128 M = (unsigned __int128)m;
        unsigned __int128 N = M * M * 5;
        unsigned __int128 r = isqrt_u128(N);
        unsigned __int128 f = (M + r) / 2;
        return u128_to_seq(f + M + 2);
    }
}

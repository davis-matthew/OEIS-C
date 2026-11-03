/*
 * A004935  a(n) = floor(n*phi^20), where phi is the golden ratio, A001622.
 * https://oeis.org/A004935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*phi^20) = floor((15127*n +
 *            floor(sqrt(5*6765^2*n^2)))/2), since phi^20 = (L(20) +
 *            F(20)*sqrt(5))/2 = (15127 + 6765*sqrt(5))/2
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 42 terms through n = 8901 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

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

static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 r, y;
    if (x < 2) return x;
    r = x;
    y = (r + 1) / 2;
    while (y < r) {
        r = y;
        y = (r + x / r) / 2;
    }
    return r;
}

static seq_t isqrt_seq(seq_t x) {
    seq_t r, y;
    if (x < 2) return x;
    r = x;
    y = (r + 1) / 2;
    while (y < r) {
        r = y;
        y = (r + x / r) / 2;
    }
    return r;
}

seq_t a(idx_t n) {
    if (seq_is_big()) {
        seq_t N = (seq_t)n;
        seq_t s = isqrt_seq(N * N * 228826125);
        return (N * 15127 + s) / 2;
    } else {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 s = isqrt_u128(N * N * 228826125);
        return u128_to_seq((N * 15127 + s) / 2);
    }
}

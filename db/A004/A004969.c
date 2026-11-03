/*
 * A004969  a(n) = ceiling(n*phi^14), where phi is the golden ratio, A001622.
 * https://oeis.org/A004969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^14) = floor((843*n +
 *            floor(377*n*sqrt(5)))/2) + 1 for n >= 1, a(0) = 0, using phi^14
 *            = (L(14) + F(14)*sqrt(5))/2 = (843 + 377*sqrt(5))/2
 * Range:     u8 n <= 0, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-01)
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
    if (n == 0) return 0;
    if (seq_is_big()) {
        seq_t k = (seq_t)n * 377;
        seq_t m = k * k * 5;
        seq_t r = isqrt_seq(m);
        seq_t x = (seq_t)n * 843 + r;
        return x / 2 + 1;
    } else {
        unsigned __int128 k = (unsigned __int128)n * 377;
        unsigned __int128 m = k * k * 5;
        unsigned __int128 r = isqrt_u128(m);
        unsigned __int128 x = (unsigned __int128)n * 843 + r;
        return u128_to_seq(x / 2 + 1);
    }
}

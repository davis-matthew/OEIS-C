/*
 * A086117  Denominator of mean deviation of a symmetrical binomial
 *          distribution on n elements.
 * https://oeis.org/A086117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the mean deviation of a symmetric binomial
 *            distribution on n elements. a(n) = 2^{ n - v₂( n * C(n-1, ⌊n/2⌋
 *            ) ) }, where v₂(m) is the exponent of 2 in m
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   tz_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/tz_idx.h"

static inline int popcnt_idx(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* offset is 1; n≥1 */
    idx_t m = n >> 1;            /* floor(n/2) */
    idx_t n1 = n - 1;
    int v2_n   = tz_idx(n);                                 /* v₂(n) */
    int v2_binom = popcnt_idx(m) + popcnt_idx(n1 - m) - popcnt_idx(n1);
    int exp = (int)n - (v2_n + v2_binom);        /* exponent of 2 in denominator */

    seq_t res = 1;
    for (int i = 0; i < exp; ++i)
        res <<= 1;        /* shift left by 1 (multiplication by 2) */

    return res;
}

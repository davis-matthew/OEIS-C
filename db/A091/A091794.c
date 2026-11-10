/*
 * A091794  First column of A091793.
 * https://oeis.org/A091794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A061431(n+1) + 1 + n*(n+1)/2, where A061431(m) =
 *            lcm(m(m-1)/2+1, ..., m(m+1)/2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A091794_gcd(seq_t x, seq_t y)
{
    while (y != (seq_t)0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    idx_t m = n + 1;
    const seq_t start = (seq_t)m * ((seq_t)m - (seq_t)1) / (seq_t)2 + (seq_t)1;
    seq_t res = start;
    for (idx_t i = 1; i < m; ++i) {
        const seq_t cur = start + (seq_t)i;
        const seq_t g = A091794_gcd(res, cur);
        res = (res / g) * cur;
    }
    return res + start;
}

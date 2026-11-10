/*
 * A086197  Incorrect version of A380264.
 * https://oeis.org/A086197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = numerator((1 + Sum_{k=1..n} A051903(k))/(n+1)),
 *            A051903(1)=0
 * Range:     i8 n <= 72, i16 n <= 72, i32 n <= 72, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-07)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static seq_t A051903_a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    seq_t m = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)ex[i];
        if (e > m)
            m = e;
    }
    return m;
}

static seq_t A086197_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t s = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        s += A051903_a(k);
    seq_t d = (seq_t)n + 1;
    seq_t g = A086197_gcd(s, d);
    return s / g;
}

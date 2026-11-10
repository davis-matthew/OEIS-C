/*
 * A086198  Incorrect version of A380265.
 * https://oeis.org/A086198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = denominator((1 + Sum_{k=1..n} A051903(k))/(n+1))
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-07)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static seq_t A051903_a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    seq_t maxexp = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)ex[i];
        if (e > maxexp)
            maxexp = e;
    }
    return maxexp;
}

seq_t a(idx_t n)
{
    seq_t num = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        num += A051903_a(k);
    seq_t den = (seq_t)n + 1;
    seq_t g = gcd(num, den);
    return den / g;
}

/*
 * A083055  a(n) = cardinality{ k<=n / A083054(k)=1}.
 * https://oeis.org/A083055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k<=n : floor(k*sqrt(3)) mod 3 = 1 } = Sum_{k=1..n}
 *            [A083054(k)=1], A083054(k) = floor(sqrt(3*k^2)) mod 3
 * Range:     u8 n <= 11, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-07)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"

static seq_t A083054_a(idx_t n)
{
    if (n == 0) return 0;
    seq_t N = (seq_t)n;
    seq_t prod = N * N;
    prod = prod * ((seq_t)3);
    seq_t sq = isqrt_seq(prod);
    return sq % ((seq_t)3);
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; k++) {
        if (A083054_a(k) == (seq_t)1) count++;
    }
    return count;
}

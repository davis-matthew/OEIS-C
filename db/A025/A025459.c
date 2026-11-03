/*
 * A025459  Number of partitions of n into 6 positive cubes.
 * https://oeis.org/A025459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 6 positive cubes
 *            Equivalent to the coefficient of x^n y^6 in ∏_{k≥1} 1/(1−y
 *            x^{k^3})
 * Range:     u8 n <= 624, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 124 terms through n = 8166 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const idx_t PARTS = 6ULL;
    size_t stride = (size_t)(n + 1);
    size_t dim = (size_t)(PARTS + 1) * stride;
    seq_t *dp = mem_new(seq_t, dim);
    if (!dp)
        return (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t k = 1; ; ++k) {
        seq_t cube_seq = (seq_t)k * (seq_t)k * (seq_t)k;
        if (cube_seq > (seq_t)n)
            break;
        idx_t cube = (idx_t)cube_seq;
        for (idx_t s = cube; s <= n; ++s) {
            size_t idx = (size_t)s;
            for (idx_t i = 1; i <= PARTS; ++i) {
                size_t pos  = (size_t)i * stride + idx;
                size_t prev = (size_t)(i - 1) * stride + (idx - cube);
                dp[pos] += dp[prev];
            }
        }
    }

    seq_t result = dp[(size_t)PARTS * stride + (size_t)n];
    mem_free(dp);
    return result;
}

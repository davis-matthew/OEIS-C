/*
 * A173441  Number of divisors d of n such that sigma(d) divides n.
 * https://oeis.org/A173441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = number of divisors d of n such that sigma(d) divides n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

static void dfs(const uint64_t *pr, const uint64_t *ex, size_t idx, size_t cnt,
                uint64_t cur_div, uint64_t cur_sigma, uint64_t N,
                uint64_t *cnt_out)
{
    if (idx == cnt) {
        if (N % cur_sigma == 0)
            (*cnt_out)++;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t maxe = ex[idx];
    uint64_t powp = 1;          /* pᵉ */
    uint64_t sum_sigma = 1;     /* 1 + p + … + pᵉ */
    for (uint64_t e = 0; e <= maxe; ++e) {
        dfs(pr, ex, idx + 1, cnt,
            cur_div * powp,
            cur_sigma * sum_sigma,
            N, cnt_out);
        powp *= p;
        sum_sigma += powp;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);
    uint64_t result = 0;
    dfs(primes, exps, 0, cnt, 1, 1, N, &result);
    return (seq_t)result;
}

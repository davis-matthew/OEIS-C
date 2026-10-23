/*
 * A134681  Number of digits of all the divisors of n.
 * https://oeis.org/A134681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} (floor(log10(d))+1) // number of digits of all
 *            divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
/* helpers from the utils library */
#include "../utils/factor_u64.h"
#include "../utils/digit_count.h"

/* recursive helper to enumerate all divisors and accumulate their digit counts */
static void
dfs(const uint64_t *pr, const uint64_t *ex, size_t cnt,
    size_t idx, uint64_t cur, seq_t *sum)
{
    if (idx == cnt) {
        *sum += (seq_t)digit_count(cur);
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = cur;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        dfs(pr, ex, cnt, idx + 1, val, sum);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(nn, pr, ex, &cnt);
    seq_t sum = 0;
    dfs(pr, ex, cnt, 0, 1ULL, &sum);
    return sum;
}

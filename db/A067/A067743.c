/*
 * A067743  Number of divisors of n not in the half-open interval [sqrt(n/2),
 *          sqrt(n*2)).
 * https://oeis.org/A067743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of divisors d of n with d^2 ≤ n/2 or d^2 > 2n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void dfs_div(idx_t cur, int idx, idx_t nidx, idx_t two_n,
                    seq_t *cnt, uint64_t *pr, uint64_t *ex, size_t pcount)
{
    if ((size_t)idx == pcount) {
        idx_t d = cur;
        idx_t d2 = d * d;
        idx_t two_d2 = 2 * d2;
        if (two_d2 <= nidx || d2 > two_n)
            (*cnt)++;
        return;
    }

    idx_t val = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        dfs_div(cur * val, idx + 1, nidx, two_n, cnt, pr, ex, pcount);
        if (e == ex[idx]) break;
        val *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    /* factor_u64 is provided by the utilities of the build environment */
    uint64_t pr[64];          /* primes in factorization */
    uint64_t ex[64];          /* corresponding exponents */
    size_t pf_cnt;            /* number of distinct prime factors */

    factor_u64(n, pr, ex, &pf_cnt);

    seq_t count = 0;
    idx_t nidx  = n;
    idx_t two_n = 2 * nidx;

    dfs_div(1, 0, nidx, two_n, &count, pr, ex, pf_cnt);
    return count;
}

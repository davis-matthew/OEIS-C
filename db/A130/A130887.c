/*
 * A130887  Inverse Moebius transform of the Mersenne numbers: a(n) =
 *          Sum_{d|n} (2^d - 1).
 * https://oeis.org/A130887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (2^d - 1) (OEIS A130887)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp) */
#include "../utils/factor_u64.h" /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* Recursive helper: visit all divisors of n. */
static void dfs_div(idx_t pos,
                    idx_t cur,
                    const uint64_t *primes,
                    const uint64_t *exps,
                    size_t cnt,
                    seq_t *sum)
{
    if (pos == cnt) {
        *sum += pow2(cur);
        return;
    }

    idx_t val = cur;
    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        dfs_div(pos + 1, val, primes, exps, cnt, sum);
        val *= primes[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, guard against underflow */

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute τ(n) = number of divisors */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (exps[i] + 1);

    /* Sum 2^d over all divisors d of n */
    seq_t sum = 0;
    dfs_div(0, (idx_t)1, primes, exps, cnt, &sum);

    /* a(n) = sum_{d|n}2^d - τ(n) */
    sum -= (seq_t)tau;
    return sum;
}

/*
 * A206764  a(n) = Sum_{k=1..n} binomial(n,k) * sigma(n,k) * (-1)^(n-k).
 * https://oeis.org/A206764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * sigma(n,k) * (-1)^(n-k) /*
 *            where sigma(n,k) is the sum of the k-th powers of the divisors
 *            of n. The implementation uses the equivalent expression a(n) =
 *            (-1)^n * ( Sum_{d|n} (1-d)^n - tau(n) ), where tau(n) is the
 *            number of divisors of n
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

static void dfs_dfs(idx_t idx, idx_t factor_cnt, const uint64_t *primes,
                    const uint64_t *exps, seq_t cur_div, seq_t *sum_term,
                    size_t *tau_cnt, idx_t n)
{
    if (idx == factor_cnt) {
        /* reached a complete divisor */
        (*tau_cnt)++;
        seq_t base = (seq_t)1 - cur_div;
        seq_t term = seq_pow(base, n);
        *sum_term += term;
        return;
    }

    uint64_t p = primes[idx];
    uint64_t e = exps[idx];
    seq_t val = cur_div;
    for (uint64_t i = 0; i <= e; ++i) {
        dfs_dfs(idx + 1, factor_cnt, primes, exps, val,
                sum_term, tau_cnt, n);
        val *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return 0;                         /* undefined for offset 0 */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t factor_cnt;
    factor_u64((uint64_t)n, primes, exps, &factor_cnt);

    seq_t sum_term = 0;
    size_t tau_cnt = 0;
    dfs_dfs(0, (idx_t)factor_cnt, primes, exps,
            (seq_t)1, &sum_term, &tau_cnt, n);

    seq_t sign = (n & 1ULL) ? (seq_t)-1 : (seq_t)1;
    return sign * (sum_term - (seq_t)tau_cnt);
}

/*
 * A180253  Call two divisors of n adjacent if the larger is a prime times the
 *          smaller. a(n) is the sum of elements of all pairs of adjacent
 *          divisors of n.
 * https://oeis.org/A180253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * Sum_{p|d} (1 + 1/p) (OEIS A180253)
 * Range:     u8 n <= 41, u16 n <= 4199, u32 n <= 9703, u64 n <= 9703,
 *            u128 n <= 9703, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Recursive helper that enumerates all divisors of n.
 * cur_d          : the divisor constructed so far
 * sum_num,sum_den: numerator/denominator of the sum
 *                 Sum_{p|cur_d} (1 + 1/p)   (stored as a rational)
 * res            : cumulative result (by reference)
 */
static void dfs(size_t idx,
                seq_t cur_d,
                seq_t sum_num,
                seq_t sum_den,
                const uint64_t *primes,
                const uint64_t *exponents,
                size_t prime_cnt,
                seq_t *res)
{
    if (idx == prime_cnt) {
        /* term = cur_d * sum_num / sum_den  (exact division) */
        *res += cur_d * sum_num / sum_den;
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t e   = exponents[idx];

    /* Case 1: exponent 0 – prime not present in this divisor */
    dfs(idx + 1, cur_d, sum_num, sum_den, primes, exponents, prime_cnt, res);

    /* pre‑compute the rational contribution of this prime
     * Sum_{p|cur_d} (1 + 1/p)  adds (p+1)/p  once when the prime appears
     */
    seq_t new_sum_num = sum_num * (seq_t)p + sum_den * ((seq_t)p + 1);
    seq_t new_sum_den = sum_den * (seq_t)p;

    /* Case 2: exponent >=1 – the prime is present.
     * For all exponents >0 the inner sum is identical, only cur_d changes.
     */
    seq_t d_mult = cur_d;
    for (uint64_t t = 1; t <= e; ++t) {
        d_mult *= (seq_t)p;
        dfs(idx + 1, d_mult, new_sum_num, new_sum_den,
            primes, exponents, prime_cnt, res);
    }
}

/* Public function as required by the interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range values */

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    seq_t result = 0;
    dfs(0, (seq_t)1, (seq_t)0, (seq_t)1,
        primes, exps, prime_cnt, &result);
    return result;
}

/*
 * A072501  Ratio of the product of divisors of n which are > n^(1/2) to
 *          product of divisors of n which are < n^(1/2).
 * https://oeis.org/A072501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of divisors > √n / product of divisors < √n
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 239, u64 n <= 1259,
 *            u128 n <= 5039, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Recursively generate all divisors d of n, keeping their prime exponents
   in cur_exp. For each divisor with d < √n accumulate
   sum_exp[p] += e_p - 2 * cur_exp[p].                         */
static void gen_divs(size_t idx, uint64_t curr, uint64_t *cur_exp,
                     const uint64_t *primes, const uint64_t *exps,
                     size_t cnt, uint64_t n, int64_t *sum_exp)
{
    if (idx == cnt) {
        if (((__uint128_t)curr * curr) < n) {
            for (size_t i = 0; i < cnt; ++i) {
                sum_exp[i] += (int64_t)exps[i] - 2LL * (int64_t)cur_exp[i];
            }
        }
        return;
    }

    uint64_t d = curr;
    for (uint64_t k = 0; k <= exps[idx]; ++k) {
        cur_exp[idx] = k;
        gen_divs(idx + 1, d, cur_exp, primes, exps, cnt, n, sum_exp);
        if (k < exps[idx]) {
            d = (uint64_t)(((__uint128_t)d * primes[idx]));
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t cur_exp[64] = {0};
    int64_t  sum_exp[64] = {0};

    gen_divs(0, (uint64_t)1, cur_exp, primes, exps, cnt,
             (uint64_t)n, sum_exp);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (sum_exp[i] > 0)
            result = seq_mul(result,
                             seq_pow((seq_t)primes[i], (idx_t)sum_exp[i]));
    }
    return result;
}

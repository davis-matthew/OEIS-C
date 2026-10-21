/*
 * A069626  Number of sets of integers larger than one whose least common
 *          multiple is n.
 * https://oeis.org/A069626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{ d divides n } mu(n/d) * 2^(tau(d)-1)
 * Range:     u8 n <= 47, u16 n <= 179, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where mu is the Möbius function and tau is the divisor-count function. */
/* This implementation factors n, enumerates all divisors via recursion, and
   evaluates the sum exactly using the utilities provided by the framework. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

static void rec(size_t idx, size_t primes_cnt,
                const uint64_t *exps,
                seq_t cur_tau, bool mu_zero, int mu_sign,
                seq_t *pos_sum, seq_t *neg_sum)
{
    if (idx == primes_cnt) {
        if (mu_zero) return;
        seq_t term = pow2((idx_t)(cur_tau - 1));
        if (mu_sign > 0)
            *pos_sum += term;
        else
            *neg_sum += term;
        return;
    }
    for (uint64_t ki = 0; ki <= exps[idx]; ++ki) {
        seq_t new_tau = cur_tau * (ki + 1);
        bool new_mu_zero = mu_zero;
        int new_mu_sign = mu_sign;
        uint64_t diff = exps[idx] - ki;
        if (diff > 1) {
            new_mu_zero = true;
        } else if (diff == 1) {
            new_mu_sign = -new_mu_sign;
        }
        rec(idx + 1, primes_cnt, exps, new_tau,
            new_mu_zero, new_mu_sign,
            pos_sum, neg_sum);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;        /* special case: empty set */
    /* factorization */
    size_t cnt = 0;
    uint64_t primes[64], exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    rec(0, cnt, exps, (seq_t)1, false, 1, &pos_sum, &neg_sum);
    return pos_sum - neg_sum;
}

/*
 * A129139  a(n) = number of positive integers which are coprime to n and are
 *          <= d(n), where d(n) = A000005(n).
 * https://oeis.org/A129139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * floor(tau(n)/d)
 * Range:     u8 n <= 9540, u16 n <= 9540, u32 n <= 9540, u64 n <= 9540,
 *            u128 n <= 9540, big any n
 * Helpers:   divisor_count_u64, factor_u64, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(n) is the number of divisors of n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mu_of.h"
#include "../utils/divisor_count_u64.h"

static void rec_divisors(idx_t          idx,
                         uint64_t        cur,
                         uint64_t        tau_n,
                         uint64_t const *primes,
                         uint64_t const *exps,
                         size_t          cnt,
                         int64_t        *sum)
{
    if (idx == cnt) {
        int mu = mu_of((idx_t)cur);
        if (mu == 0) return;
        uint64_t term = tau_n / cur;          /* floor division, <= tau_n */
        if (mu > 0) *sum += (int64_t)term;
        else *sum -= (int64_t)term;
        return;
    }
    uint64_t prod = cur;
    /* iterate over exponent 0..exps[idx] */
    for (uint64_t k = 0; k <= exps[idx]; ++k) {
        rec_divisors(idx + 1, prod, tau_n, primes, exps, cnt, sum);
        if (k < exps[idx]) { /* multiply by prime once for next exponent */
            prod *= primes[idx];
        }
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification */
    uint64_t tau = divisor_count_u64((uint64_t)n);

    /* factorization: use arrays big enough for all small n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    int64_t sum = 0;
    rec_divisors(0, 1ULL, tau, primes, exps, cnt, &sum);

    /* result fits in seq_t; sum is guaranteed non‑negative */
    return (seq_t)sum;
}

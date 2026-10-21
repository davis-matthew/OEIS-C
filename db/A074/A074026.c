/*
 * A074026  Duplicate of A054663.
 * https://oeis.org/A074026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (1/n)*sum_{d|n} mu(n/d) * 5^{d-1} * (5 if (n/d) % 5 == 0
 *            else 1)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* generate all divisors d of n and accumulate the required sum */
static void dfs_divisors(const uint64_t *primes, const uint64_t *exp_n,
                         int cnt, int idx, idx_t cur_d,
                         int primes_in_k, bool zero_k,
                         idx_t n_total, seq_t *sum)
{
    if (idx == cnt) {
        if (!zero_k) {
            /* compute 5^(cur_d-1) */
            seq_t pow5 = big_pow((seq_t)5, (seq_t)(cur_d - 1));
            /* multiply by 5 if (n/d) is a multiple of 5 */
            if ((n_total / cur_d) % 5 == 0)
                pow5 = pow5 * (seq_t)5;
            /* Möbius value */
            int mu = (primes_in_k % 2 == 0) ? 1 : -1;
            *sum += (seq_t)mu * pow5;
        }
        return;
    }

    uint64_t p = primes[idx];
    uint64_t exp = exp_n[idx];
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= exp; ++e) {
        int new_primes_in_k = primes_in_k;
        bool new_zero_k = zero_k;
        uint64_t e_prime = exp - e;          /* exponent in k = n / d */
        if (e_prime > 1)
            new_zero_k = true;
        else if (e_prime == 1)
            new_primes_in_k++;

        dfs_divisors(primes, exp_n, cnt, idx + 1,
                     cur_d * mult, new_primes_in_k, new_zero_k,
                     n_total, sum);
        mult *= p;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* factor n */
    uint64_t primes[64];
    uint64_t exp_n[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exp_n, &cnt);

    seq_t sum = (seq_t)0;
    dfs_divisors(primes, exp_n, (int)cnt, 0, 1, 0, false, n, &sum);

    return sum / (seq_t)n;
}

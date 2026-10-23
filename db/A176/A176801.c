/*
 * A176801  a(n) is the smallest natural number m such that product of
 *          arithmetic mean of the divisors of n and arithmetic mean of the
 *          divisors of m is an integer.
 * https://oeis.org/A176801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that (σ(n)/τ(n))*(σ(m)/τ(m)) is integer,
 *            where σ(x) is the sum of divisors of x and τ(x) is the divisor
 *            count
 * Range:     u8 n <= 15, u16 n <= 9687, u32 n <= 9687, u64 n <= 9687,
 *            u128 n <= 9687, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Helper: compute σ(n) and τ(n) for n >= 1.
   Result is stored in *sigma and *tau.    */
static void compute_sigma_tau(idx_t n, seq_t *sigma, seq_t *tau)
{
    /* For factor_u64 we need arrays large enough for all prime factors.
       The maximum number of distinct prime factors of a 64‑bit integer is < 16. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Use 128‑bit arithmetic for intermediate values to avoid overflow
       for n up to at least 10^12. */
    unsigned __int128 sum;
    unsigned __int128 pow;
    unsigned __int128 sigma_val = 1;
    unsigned long long tau_val = 1;

    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long p = primes[i];
        unsigned long long e = exps[i];

        /* σ(p^e) = 1 + p + p^2 + … + p^e  */
        sum = 1;
        pow = 1;
        for (unsigned long long j = 1; j <= e; ++j) {
            pow *= p;
            sum += pow;
        }
        sigma_val *= sum;
        tau_val   *= (e + 1);
    }

    /* For n = 1, factor_u64 returns cnt = 0; σ = τ = 1 is already set. */
    *sigma = (seq_t)sigma_val;
    *tau   = (seq_t)tau_val;
}

seq_t a(idx_t n)
{
    seq_t sigma_n, tau_n;
    compute_sigma_tau(n, &sigma_n, &tau_n);

    /* If σ(n)/τ(n) is already an integer, the answer is 1. */
    if (sigma_n % tau_n == 0)
        return 1;

    for (idx_t m = 2;; ++m) {
        seq_t sigma_m, tau_m;
        compute_sigma_tau(m, &sigma_m, &tau_m);

        seq_t prod   = sigma_n * sigma_m;
        seq_t denom  = tau_n   * tau_m;

        if (prod % denom == 0)
            return m;
    }
}

/*
 * A062778  Values of Moebius-transform of PrimePi function.
 * https://oeis.org/A062778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} pi(n/d) * mu(d)
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility headers */
#include "../utils/factor_u64.h"
#include "../utils/mu_of.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS offset; nothing required for n==0 */
    uint64_t nn = (uint64_t)n;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_cnt = 0;
    factor_u64(nn, primes, exps, &factor_cnt);

    /* Generate all divisors of n */
    uint64_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;   /* divisor 1 */

    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t p    = primes[i];
        uint64_t power = 1;
        size_t old_cnt = div_cnt;
        for (size_t e = 1; e <= exps[i]; ++e) {
            power *= p;
            for (size_t j = 0; j < old_cnt; ++j) {
                divs[div_cnt++] = divs[j] * power;
            }
        }
    }

    /* Iterate divisors to accumulate the sum */
    int64_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        int mu = mu_of((idx_t)d);
        if (mu == 0) continue;
        seq_t pi_val = compute_pi(nn / d);
        int64_t term = (int64_t)pi_val;
        sum += mu > 0 ? term : -term;
    }

    return (seq_t)sum;
}

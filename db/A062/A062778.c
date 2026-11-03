/*
 * A062778  Values of Moebius-transform of PrimePi function.
 * https://oeis.org/A062778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} pi(n/d) * mu(d)
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 9789 (2026-11-01)
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility headers */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

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
        seq_t pi_val = prime_pi(nn / d);
        int64_t term = (int64_t)pi_val;
        sum += mu > 0 ? term : -term;
    }

    return (seq_t)sum;
}

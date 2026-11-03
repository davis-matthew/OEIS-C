/*
 * A236287  a(n) = sigma(n)^tau(n), where tau(n) = A000005(n) = the number of
 *          divisors of n and sigma(n) = A000203(n) = the sum of divisors of
 *          n.
 * https://oeis.org/A236287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n)^tau(n) where sigma(n) is the sum of divisors of
 *            n (A000203) and tau(n) is the number of divisors of n (A000005)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 47, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 100 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation directly factors n on each call
   using factor_u64 and computes sigma(n) and tau(n)
   multiplicatively, then takes the power with big_pow. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard; offset starts at 1 */

    /* Factor n.  n fits in uint64_t because idx_t is uint64_t. */
    uint64_t num = (uint64_t)n;
    uint64_t primes[32];                /* enough for n <= 2^64-1 */
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    /* Compute sigma(n) and tau(n) multiplicatively. */
    seq_t sigma = 1;
    seq_t tau   = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];

        /* sigma(p^e) = (p^(e+1) - 1) / (p - 1) */
        seq_t term = ipow(p, e + (seq_t)1);
        term = (term - (seq_t)1) / (p - (seq_t)1);
        sigma *= term;

        /* tau(p^e) = e + 1 */
        tau *= (e + (seq_t)1);
    }

    /* Final value a(n) = sigma(n)^tau(n) */
    return ipow(sigma, tau);
}

/*
 * A019530  Smallest number m such that m^m is divisible by n.
 * https://oeis.org/A019530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest m such that m^m is divisible by n. (For n=1,
 *            a(1)=0 by definition.)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    /* Factor n: store primes and their exponents. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the product of distinct primes dividing n. */
    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= primes[i];
    }
    if (prod == 0) prod = 1; /* safety, though prod cannot be 0 here */

    /* Search m in steps of prod, since m must be divisible by all primes of n. */
    for (idx_t m = prod; m <= n; m += prod) {
        bool ok = true;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t e = exps[i];

            /* Count exponent of p in m. */
            uint64_t tmp = m;
            uint64_t k = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++k;
            }

            if (k == 0) {          /* m not divisible by p */
                ok = false;
                break;
            }

            /* Check m * k >= e without overflow:
               equivalent to m >= ceil(e / k). */
            seq_t threshold = ((seq_t)e + k - 1) / k;
            if ((seq_t)m < threshold) {
                ok = false;
                break;
            }
        }
        if (ok) {
            return (seq_t)m;
        }
    }

    /* Fallback: for completeness, should never reach here. */
    return (seq_t)n;
}

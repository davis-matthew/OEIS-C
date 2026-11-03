/*
 * A236286  a(n) = tau(n)^sigma(n) / tau(n)^n, where tau(n) = A000005(n) = the
 *          number of divisors of n and sigma(n) = A000203(n) = the sum of
 *          divisors of n.
 * https://oeis.org/A236286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n)^(sigma(n)-n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 11, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 50 (2026-11-01)
 * Helpers:   ipow, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau     = A000005(n) = number of divisors of n         */
/*       sigma   = A000203(n) = sum of divisors of n           */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* n must be >= 1; the OEIS offset is 1. */
    seq_t tau   = num_divisors((uint64_t)n);   /* tau(n)   */
    seq_t sigma = sigma_of((uint64_t)n);            /* sigma(n) */
    seq_t exp   = sigma - (seq_t)n;                 /* sigma(n)-n */
    return ipow(tau, exp);                      /* tau(n)^exp */
}

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
 * Helpers:   big_pow, divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau     = A000005(n) = number of divisors of n         */
/*       sigma   = A000203(n) = sum of divisors of n           */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n must be >= 1; the OEIS offset is 1. */
    seq_t tau   = divisor_count_u64((uint64_t)n);   /* tau(n)   */
    seq_t sigma = sigma_of((uint64_t)n);            /* sigma(n) */
    seq_t exp   = sigma - (seq_t)n;                 /* sigma(n)-n */
    return big_pow(tau, exp);                      /* tau(n)^exp */
}

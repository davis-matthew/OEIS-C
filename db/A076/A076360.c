/*
 * A076360  a(n) = d(sigma(n)) - sigma(d(n)), where d(n) is the number of
 *          divisors of n and sigma(n) is their sum.
 * https://oeis.org/A076360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = d(sigma(n)) - sigma(d(n))
 * Range:     i8 n <= 8399, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_and_compute, sigma_of_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_and_compute.h"
#include "../utils/sigma_of_small.h"

/* Main sequence function.  It may be called with any order of indices. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, return 0 */

    uint64_t nn = (uint64_t)n;
    uint64_t tau_n, sigma_n;
    factor_and_compute(nn, &tau_n, &sigma_n);

    uint64_t tau_sigma, dummy;
    factor_and_compute(sigma_n, &tau_sigma, &dummy);

    uint64_t sigma_tau;
    /* tau_n is small, we can compute its sigma by factoring directly. */
    factor_and_compute(tau_n, &dummy, &sigma_tau);

    /* The difference may be negative, use signed integer arithmetic. */
    int64_t result = (int64_t)tau_sigma - (int64_t)sigma_tau;
    return (seq_t)result;
}

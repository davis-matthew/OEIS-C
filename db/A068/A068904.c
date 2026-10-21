/*
 * A068904  a(n) = binomial(sigma(n),tau(n)), where sigma(n) is the sum and
 *          tau(n) the number of divisors of n (A000203, A000005).
 * https://oeis.org/A068904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(sigma(n), tau(n))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 83, u128 n <= 239,
 *            big any n
 * Helpers:   binomial, divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n >= 1 by the definition of the sequence */
    seq_t sigma = sigma_of(n);                /* σ(n) */
    uint64_t tau = divisor_count_u64((uint64_t)n); /* τ(n) */

    return binomial((idx_t)sigma, (idx_t)tau);
}

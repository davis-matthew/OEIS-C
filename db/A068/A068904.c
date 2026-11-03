/*
 * A068904  a(n) = binomial(sigma(n),tau(n)), where sigma(n) is the sum and
 *          tau(n) the number of divisors of n (A000203, A000005).
 * https://oeis.org/A068904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(sigma(n), tau(n))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 29, u64 n <= 83, u128 n <= 239,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 7989 (2026-11-01)
 * Helpers:   binomial, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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
    /* n >= 1 by the definition of the sequence */
    seq_t sigma = sigma_of(n);                /* σ(n) */
    uint64_t tau = num_divisors((uint64_t)n); /* τ(n) */

    return binomial((idx_t)sigma, (idx_t)tau);
}

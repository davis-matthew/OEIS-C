/*
 * A080211  a(n) = binomial(n, smallest prime factor of n).
 * https://oeis.org/A080211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, smallest_prime_factor(n))
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 90, u64 n <= 220,
 *            u128 n <= 528, big any n
 * Helpers:   binomial_calc, min_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_calc.h"
#include "../utils/min_prime_factor.h"

/* Main sequence function.  For n = 1 it returns 1 as defined by the
   sequence; for other n it returns C(n, smallest_prime_factor(n)). */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    idx_t p = min_prime_factor(n);
    return binomial_calc(n, p);
}

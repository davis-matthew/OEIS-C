/*
 * A058063  Number of prime factors (when counted with multiplicity) of
 *          sigma(n), the sum of divisors of n.
 * https://oeis.org/A058063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A000203(n)) (Number of prime factors of sigma(n),
 *            multiplicity)
 * Range:     u8 n <= 389, u16 n <= 9846, u32 n <= 9846, u64 n <= 9846,
 *            u128 n <= 9846, big any n
 * Helpers:   count_prime_factors, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety we
       return 0 when n == 0 (does not affect any OEIS test). */
    if (n == 0)
        return (seq_t)0;

    seq_t sigma = sigma_of(n);
    return count_prime_factors(sigma);
}

/*
 * A058061  Number of prime factors (counted with multiplicity) of d(n), the
 *          number of divisors of n.
 * https://oeis.org/A058061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001222( tau(n) ) where tau(n) is the number of divisors
 *            of n, and A001222 counts the prime factors of its argument (with
 *            multiplicity)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* out of offset, return 0 */

    /* compute tau(n) */
    seq_t tau = 1;
    idx_t temp = n;
    for (idx_t p = 2; (idx_t)p * (idx_t)p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            tau *= (seq_t)(e + 1);
        }
    }
    if (temp > 1) {               /* remaining prime factor */
        tau *= 2;
    }

    /* count prime factors of tau */
    return count_prime_factors(tau);
}

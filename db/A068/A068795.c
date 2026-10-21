/*
 * A068795  In prime factorization of n replace all primes with the greatest
 *          prime factor of n; a(1)=1.
 * https://oeis.org/A068795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GPF(n)^bigomega(n) where GPF(n) is the greatest prime
 *            factor of n and bigomega(n) is the total number of prime factors
 *            (with multiplicity). a(1) = 1. This follows from the OEIS
 *            description: replace each prime in the factorization of n with
 *            the greatest prime factor of n
 * Range:     u8 n <= 27, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   big_pow, bigomega, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In order to use the library helpers we need to include the
   appropriate headers:
   - largest_prime_factor(uint64_t)   -> "../utils/largest_prime_factor.h"
   - bigomega(idx_t)                  -> "../utils/bigomega.h"
   - big_pow(seq_t, seq_t)            -> "../utils/big_pow.h"
*/

#include <stdint.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Get greatest prime factor (GPF) of n */
    uint64_t gpf = largest_prime_factor((uint64_t)n);
    /* Get number of prime factors counted with multiplicity */
    seq_t omega = bigomega(n);

    /* Compute gpf^omega using arbitrary precision power routine. */
    return big_pow((seq_t)gpf, omega);
}

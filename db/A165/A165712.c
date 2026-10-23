/*
 * A165712  a(n) = the smallest integer > n that is divisible by exactly the
 *          same number of primes (counted with multiplicity) as n is.
 * https://oeis.org/A165712
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165712
 *  a(n) = the smallest integer > n that has exactly the same number of
 *  prime factors (counted with multiplicity) as n.  The value Omega(n)
 *  is obtained via the helper function count_prime_factors(seq_t).
 */

#include "../utils/count_prime_factors.h"

/* Compute a(n) for n >= 2.  Behaviour for n < 2 is undefined. */
seq_t a(idx_t n)
{
    /* Omega(n): number of prime factors of n with multiplicity. */
    seq_t target = count_prime_factors((seq_t)n);

    /* Search for the least integer > n with the same Omega value. */
    for (idx_t m = n + 1; ; ++m)
    {
        if (count_prime_factors((seq_t)m) == target)
            return (seq_t)m;   /* fits in seq_t by requirement */
    }
}

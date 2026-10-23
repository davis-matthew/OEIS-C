/*
 * A167746  Number of prime divisors of A001222 (counted with multiplicity),
 *          with a(1) = 1 by convention.
 * https://oeis.org/A167746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(bigomega(n)); a(1)=1 by convention
 * Range:     u8 n <= 1727, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;               /* convention for a(1) */

    seq_t first  = count_prime_factors((seq_t)n);  /* Ω(n) */
    seq_t second = count_prime_factors(first);     /* Ω(Ω(n)) */

    return second;
}

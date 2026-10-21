/*
 * A079067  Number of primes less than greatest prime factor of n but not
 *          dividing n.
 * https://oeis.org/A079067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(gpf(n)-1) - (num_distinct_primes(n)-1) where gpf(n) is
 *            the greatest prime factor of n. For n = 1 the value is defined
 *            as 0
 * Range:     u8 n <= 1620, u16 n <= 9985, u32 n <= 9985, u64 n <= 9985,
 *            u128 n <= 9985, big any n
 * Helpers:   compute_pi, count_distinct_primes, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* n is at least 2 */
    uint64_t val = (uint64_t)n;

    /* Greatest prime factor */
    uint64_t g = largest_prime_factor(val);

    /* Prime counting function pi(x) for x = g-1 */
    seq_t pi_gm1 = compute_pi((idx_t)(g) - 1);

    /* Number of distinct prime factors of n */
    int distinct = count_distinct_primes(val);

    /* Result: pi(g-1) - (distinct-1) */
    seq_t result = pi_gm1 - ((seq_t)(distinct - 1));

    return result;
}

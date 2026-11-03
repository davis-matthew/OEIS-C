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
 * Range:     u8 n <= 1620, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 112 terms through n = 9985 (2026-11-01)
 * Helpers:   largest_prime_factor, omega, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/omega.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* n is at least 2 */
    uint64_t val = (uint64_t)n;

    /* Greatest prime factor */
    uint64_t g = largest_prime_factor(val);

    /* Prime counting function pi(x) for x = g-1 */
    seq_t pi_gm1 = prime_pi((idx_t)(g) - 1);

    /* Number of distinct prime factors of n */
    int distinct = omega(val);

    /* Result: pi(g-1) - (distinct-1) */
    seq_t result = pi_gm1 - ((seq_t)(distinct - 1));

    return result;
}

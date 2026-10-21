/*
 * A083721  Number of primes greater than the greatest prime factor of n but
 *          not greater than n.
 * https://oeis.org/A083721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n) - pi(gpf(n)) (gpf(n) = greatest prime factor of n)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   compute_pi, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Greatest prime factor of n (>= 2 for n >= 2). */
    uint64_t gpf = largest_prime_factor((uint64_t)n);

    /* Prime counting functions produce values that fit in seq_t. */
    seq_t pi_n   = compute_pi(n);
    seq_t pi_gpf = compute_pi((idx_t)gpf);

    return pi_n - pi_gpf;
}

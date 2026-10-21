/*
 * A084294  Number of primes in the interval [prime(n),n+prime(n)].
 * https://oeis.org/A084294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pi(n+prime(n)) - n + 1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of primes in the interval [prime(n), n + prime(n)] */

#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime */
    uint32_t p = get_prime(n);

    /* Compute the upper bound of the interval */
    idx_t limit = n + (idx_t)p;

    /* Count primes up to that bound */
    seq_t pi = compute_pi(limit);

    /* Number of primes in [prime(n), n+prime(n)] */
    seq_t result = pi - (seq_t)n + (seq_t)1;
    return result;
}

/*
 * A070803  Number of primes not exceeding sum of divisors of n.
 * https://oeis.org/A070803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = pi(sigma(n))
 * Range:     u8 n <= 538, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   compute_pi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of primes not exceeding the sum of divisors of n. */

#include "../utils/sigma_of.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but 0 is harmless */
    seq_t sigma = sigma_of(n);       /* sigma(n)  */
    idx_t s = (idx_t)sigma;          /* cast to index type for pi */
    return compute_pi(s);            /* pi(sigma(n)) */
}

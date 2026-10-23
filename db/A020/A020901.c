/*
 * A020901  Greatest k such that k-th prime < 3 times n-th prime.
 * https://oeis.org/A020901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(3 * p_n) where p_n is the n-th prime and pi(x) counts
 *            primes ≤ x
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime.  n ≥ 1 is guaranteed by the caller. */
    uint32_t pn = get_prime(n);

    /* Compute the bound 3 * p_n.  Use 64‑bit arithmetic for safety. */
    idx_t limit = (idx_t)pn * 3ULL;

    /* Return the number of primes ≤ limit, i.e., the greatest k with p_k < 3*p_n. */
    return prime_pi(limit);
}

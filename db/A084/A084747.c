/*
 * A084747  Leading diagonal of triangle (shown below) in which row n lists
 *          the n smallest positive numbers k such that k + n is a prime.
 * https://oeis.org/A084747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime( primepi(n) + n ) - n
 * Range:     u8 n <= 47, u16 n <= 6290, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; this should not be called for n = 0 */
        return 0;
    /* pi(n) = number of primes ≤ n */
    seq_t pi_seq = compute_pi(n);
    idx_t pi = (idx_t)pi_seq;            /* cast fits because n is small */
    idx_t k   = pi + n;                  /* index of the required prime */
    int p     = get_prime(k);            /* k‑th prime (1‑indexed) */
    return (seq_t)p - (seq_t)n;
}

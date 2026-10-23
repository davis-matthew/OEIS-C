/*
 * A024678  a(n) is the position of (prime(n+1) + prime(n+2))/2 in the ordered
 *          nonprimes.
 * https://oeis.org/A024678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 4754, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = position of (prime(n+1)+prime(n+2))/2 in the ordered nonprimes.
   For each n ≥ 1, let m = (p_{n+1}+p_{n+2})/2, where p_k is the k‑th prime.
   Then a(n) = m – π(m), where π(m) counts the primes ≤ m. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* n must be ≥ 1; for n = 0 the OEIS offset does not apply. */
    if (n == 0) return 0;

    /* Fetch the consecutive primes p_{n+1} and p_{n+2}. */
    uint32_t p1 = get_prime(n + 1);
    uint32_t p2 = get_prime(n + 2);

    /* Their average is guaranteed integer. */
    seq_t avg = (seq_t)(p1 + p2) / 2;

    /* Convert to idx_t for the prime_pi call (safe for n up to very large values). */
    idx_t m_idx = (idx_t)avg;

    /* Number of primes ≤ avg. */
    seq_t pi = prime_pi(m_idx);

    /* Position among nonprimes: all numbers up to avg minus the primes. */
    return (seq_t)m_idx - pi;
}

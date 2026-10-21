/*
 * A079047  Number of primes between prime(n) and prime(n)^2.
 * https://oeis.org/A079047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(p_n^2 – 1) – n Count primes strictly between the n‑th
 *            prime p_n and its square p_n^2
 * Range:     u8 n <= 13, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* defined for n ≥ 1, harmless below offset */

    /* nth prime (1‑indexed) */
    idx_t p   = get_prime(n);            /* p = prime(n) */

    /* p^2 as idx_t – safe for n up to at least 10000 (p ≤ 104729) */
    idx_t p2  = p * p;

    /* number of primes ≤ p^2 – 1 */
    seq_t pi  = compute_pi(p2 - 1);

    /* subtract the n primes ≤ p_n to keep only those strictly between
       p_n and p_n^2. */
    return pi - (seq_t)n;
}

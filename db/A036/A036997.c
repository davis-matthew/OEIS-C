/*
 * A036997  Number of composite numbers <= n and relatively prime to n.
 * https://oeis.org/A036997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) - π(n) + ω(n) - 1 where φ is Euler's totient, π
 *            counts primes ≤ n, and ω is the number of distinct prime factors
 *            of n. This equals the number of composite integers ≤ n that are
 *            coprime to n
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, phi, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/phi.h"                    /* seq_t phi(idx_t m) */
#include "../utils/prime_pi.h"                /* seq_t prime_pi(idx_t limit) */
#include "../utils/count_distinct_primes.h"   /* int count_distinct_primes(uint64_t n) */

seq_t a(idx_t n)
{
    /* For any n ≥ 1.  n=0 returns 0 (behaviour below offset is irrelevant). */
    if (n < 1) return (seq_t)0;

    /* φ(n) – counts all integers 1..n coprime with n. */
    seq_t phi_n = phi(n);

    /* π(n) – counts all primes ≤ n.  Subtract those primes that divide n
       (counted by count_distinct_primes).  The remainder are the primes
       coprime with n. */
    seq_t pi_n = prime_pi(n);

    /* ω(n) – number of distinct prime factors of n. */
    int distinct = count_distinct_primes((uint64_t)n);
    seq_t distinct_seq = (seq_t)distinct;

    /* a(n) = φ(n) - π(n) + ω(n) - 1 (subtract 1 for the number 1, which is
       counted in φ but is not composite). */
    return phi_n - pi_n + distinct_seq - (seq_t)1;
}

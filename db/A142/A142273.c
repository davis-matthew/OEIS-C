/*
 * A142273  Primes congruent to 24 mod 43.
 * https://oeis.org/A142273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime that is ≡ 24 (mod 43)
 * Range:     u8 n <= 2, u16 n <= 158, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

/* Returns the n‑th term of sequence A142273.
 *  a(1) = 67, a(2) = 239, a(3) = 1013, …   (prime ≡ 24 (mod 43))   */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety for non‑positive indices */

    /* static cache of primes found so far                             */
    static seq_t *primes = NULL;          /* array of found primes     */
    static size_t primes_cap = 0;         /* current capacity         */
    static idx_t  primes_used = 0;        /* number of primes stored   */

    /* If the requested index is already cached, return it */
    if ((idx_t)primes_used >= n)
        return primes[n - 1];

    /* Next candidate that is 24 (mod 43) */
    seq_t cand = (seq_t)24;               /* 24 will become 24+43 = 67 */
    if (primes_used > 0)
        cand = primes[primes_used - 1];   /* start from last stored prime */

    cand += (seq_t)43;                    /* move to first unused candidate */

    /* Generate further primes until we have at least n of them */
    while ((idx_t)primes_used < n) {
        if (is_prime_big(cand)) {
            /* Grow the cache if needed */
            ensure_capacity(primes_used + 1, &primes_cap, (void **)&primes, sizeof(seq_t));
            primes[primes_used++] = cand;
        }
        cand += (seq_t)43;                /* next number ≡ 24 (mod 43) */
    }

    return primes[n - 1];
}

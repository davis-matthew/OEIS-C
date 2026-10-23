/*
 * A142018  Primes congruent to 14 mod 31.
 * https://oeis.org/A142018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 14 modulo 31 (OEIS A142018).
 *            The function returns a(1)=107, a(2)=293, ... It uses a simple
 *            on‑demand sieve with the helper routine is_prime_small (from the
 *            utils library) and a static cache of found terms. The cache is
 *            expanded as needed
 * Range:     u8 n <= 1, u16 n <= 215, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* the sequence is defined for n ≥ 1 only */

    static idx_t next_k   = 0;   /* next k value to test (candidate = 14+31*k) */
    static seq_t *primes = NULL;
    static size_t cap    = 0;
    static size_t stored = 0;

    while (stored < n) {
        idx_t candidate = 14 + 31 * next_k;
        next_k++;

        /* test primality of the candidate */
        if (is_prime_small((seq_t)candidate)) {
            ensure_capacity(stored + 1, &cap, (void **)&primes, sizeof(seq_t));
            primes[stored++] = (seq_t)candidate;
        }
    }

    return primes[n - 1];
}

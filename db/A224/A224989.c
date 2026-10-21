/*
 * A224989  Let p = prime(n). a(n) = number of primes q less than p, such that
 *          both p-q+1 and p-q-1 are primes.
 * https://oeis.org/A224989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A224989: a(n) = number of primes q < p(n) such that both p-q+1 and
   p-q-1 are primes, where p = prime(n). */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/sieve_primes.h"

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* nth prime */
    uint32_t p32 = get_prime((idx_t)n);
    idx_t p = (idx_t)p32;

    /* Build all primes ≤ p. */
    idx_t *primes = NULL;
    size_t prime_cnt;
    sieve_primes(p, &primes, &prime_cnt);

    seq_t count = (seq_t)0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t q = primes[i];
        if (q == p) break;          /* skip the prime p itself */

        idx_t r = p - q + (idx_t)1;
        idx_t s = p - q - (idx_t)1;

        if (is_prime(r) && is_prime(s))
            ++count;
    }

    /* free allocated memory */
    free(primes);

    return count;
}

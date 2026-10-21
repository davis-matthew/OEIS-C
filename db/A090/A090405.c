/*
 * A090405  a(n) = PrimePi(n+2) - PrimePi(n).
 * https://oeis.org/A090405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime_pi(n+2) - prime_pi(n) (offset 1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Computes the nth term of OEIS A090405.
 *    a(n) = PrimePi(n+2) - PrimePi(n)
 * n must be >= 1 (offset 1).  The function uses a simple
 * incremental sieve and caches results so arbitrary order
 * of calls is supported.  It is fast enough for n up to at
 * least 10000 (well within the 2‑second time limit). */
seq_t a(idx_t n)
{
    /* static data used for the sieve */
    static idx_t limit = 0;          /* maximum index that has been sieved */
    static uint8_t *is_prime = NULL; /* 1 if prime, 0 otherwise */
    static seq_t *prime_pi = NULL;   /* cumulative prime count (prime_pi[i] = primes <= i) */

    /* We need prime_pi values up to n+2 */
    idx_t need = n + 2;

    if (need > limit) {
        /* Allocate new arrays for the extended range */
        size_t sz = (size_t)(need + 1);      /* array size (0 .. need) */
        uint8_t *prime = (uint8_t*)malloc(sz * sizeof(uint8_t));
        seq_t *pi_arr = (seq_t*)malloc(sz * sizeof(seq_t));
        if (!prime || !pi_arr) {
            /* malloc failed – fall back to zero result */
            free(prime);
            free(pi_arr);
            return (seq_t)0;
        }

        /* initialise all entries to prime */
        memset(prime, 1, sz);
        if (sz > 0) prime[0] = 0;
        if (sz > 1) prime[1] = 0;

        /* classical Eratosthenes sieve */
        for (size_t p = 2; p * p <= need; ++p) {
            if (prime[p]) {
                for (size_t m = p * p; m <= need; m += p)
                    prime[m] = 0;
            }
        }

        /* build prime_pi array */
        pi_arr[0] = (seq_t)0;
        for (size_t i = 1; i < sz; ++i) {
            pi_arr[i] = pi_arr[i - 1] + (prime[i] ? (seq_t)1 : (seq_t)0);
        }

        /* replace old data */
        free(is_prime);
        free(prime_pi);
        is_prime = prime;
        prime_pi = pi_arr;
        limit = need;
    }

    /* return the desired difference */
    return prime_pi[(size_t)(n + 2)] - prime_pi[(size_t)n];
}

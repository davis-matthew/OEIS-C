/*
 * A076225  Counts of the maximum value in n-th row of A076221.
 * https://oeis.org/A076225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + pi(n) - pi(n/2)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts of the maximum value in the n-th row of A076221. */

#include <stddef.h>
#include <stdlib.h>

/* Static state to cache a simple sieve and prime prefix counts. */
static idx_t last_limit = 0;
static unsigned char *is_prime = NULL;   /* 0 = composite, 1 = prime */
static idx_t *pi_prefix = NULL;          /* pi_prefix[i] = number of primes <= i */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; return 0 for n==0 as a safe default. */
    if (n == 0) return 0;

    /* If we need primes up to a larger limit, rebuild the sieve. */
    if (n > last_limit) {
        idx_t new_sz = n + 1;

        /* Allocate and initialise a fresh sieve array. */
        unsigned char *new_is_prime = (unsigned char *)malloc(new_sz);
        if (!new_is_prime) return 0;          /* allocation failure */

        for (idx_t i = 0; i < new_sz; ++i) new_is_prime[i] = 1;
        if (new_sz > 0) new_is_prime[0] = 0;
        if (new_sz > 1) new_is_prime[1] = 0;

        /* Simple Eratosthenes sieve. */
        for (idx_t i = 2; i * i <= n; ++i) {
            if (new_is_prime[i]) {
                for (idx_t j = i * i; j <= n; j += i)
                    new_is_prime[j] = 0;
            }
        }

        /* Build prefix counts of primes. */
        idx_t *new_pi = (idx_t *)malloc(new_sz * sizeof(idx_t));
        if (!new_pi) {
            free(new_is_prime);
            return 0;
        }

        idx_t count = 0;
        for (idx_t i = 0; i < new_sz; ++i) {
            if (i >= 2 && new_is_prime[i]) ++count;
            new_pi[i] = count;
        }

        /* Replace the old cached data. */
        free(is_prime);
        free(pi_prefix);
        is_prime   = new_is_prime;
        pi_prefix  = new_pi;
        last_limit = n;
    }

    /* Retrieve prime count values and compute the term. */
    idx_t pi_n    = pi_prefix[n];
    idx_t pi_half = pi_prefix[n / 2];
    seq_t result  = (seq_t)(1 + pi_n - pi_half);
    return result;
}

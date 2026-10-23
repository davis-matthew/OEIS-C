/*
 * A109584  a(n) = (prime(n+1) - prime(n))^pi(n).
 * https://oeis.org/A109584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+1) - prime(n))^pi(n) (offset 1)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 29, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"      /* seq_t get_prime(uint64_t idx); */
#include "../utils/seq_pow.h"        /* seq_t seq_pow(seq_t base, idx_t exp); */

static idx_t sieve_limit = 0;
static unsigned char *sieve = NULL;          /* 0 = prime, 1 = composite */
static size_t *pi_prefix = NULL;             /* pi_prefix[i] = number of primes ≤ i */

/* Initialise (or extend) the sieve up to 'limit' inclusive.  */
static void init_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    size_t new_limit = (size_t)limit;
    unsigned char *new_sieve = (unsigned char *)calloc(new_limit + 1, 1);
    if (!new_sieve) return; /* out of memory – unlikely in test range */

    /* Mark composites */
    new_sieve[0] = new_sieve[1] = 1; /* 0 and 1 are not prime */
    for (size_t p = 2; p * p <= new_limit; ++p) {
        if (!new_sieve[p]) {
            for (size_t q = p * p; q <= new_limit; q += p)
                new_sieve[q] = 1;
        }
    }

    /* Build prefix counts for π(n) */
    size_t *new_pi = (size_t *)malloc((new_limit + 1) * sizeof(size_t));
    if (!new_pi) {
        free(new_sieve);
        return;
    }
    new_pi[0] = 0;
    for (size_t i = 1; i <= new_limit; ++i)
        new_pi[i] = new_pi[i - 1] + (size_t)(!new_sieve[i]);

    /* Replace old tables */
    free(sieve);
    free(pi_prefix);
    sieve = new_sieve;
    pi_prefix = new_pi;
    sieve_limit = new_limit;
}

/* Return π(n) for n ≥ 0.  */
static size_t pi_of(idx_t n)
{
    if (n > sieve_limit) init_sieve(n);
    return pi_prefix[(size_t)n];
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset, return 0 */

    seq_t p_n     = get_prime(n);
    seq_t p_next  = get_prime(n + 1);
    seq_t diff    = p_next - p_n;        /* positive difference */

    size_t pi_n = pi_of(n);              /* number of primes ≤ n */
    seq_t result = seq_pow(diff, (idx_t)pi_n);  /* (diff)^pi_n */

    return result;
}

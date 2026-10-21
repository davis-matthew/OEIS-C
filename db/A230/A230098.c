/*
 * A230098  a(n) = n*prime(prime(n)) - prime(n)^2.
 * https://oeis.org/A230098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*prime(prime(n)) - prime(n)^2
 * Range:     i8 n <= 7, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for primes */
static idx_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* Ensure that at least n_needed primes are stored in the cache. */
static void ensure_prime_count(idx_t n_needed)
{
    if (primes_len >= (size_t)n_needed)
        return;

    /* Start candidate from the first possible odd number after the last prime */
    idx_t candidate = (primes_len == 0) ? 2 : primes[primes_len - 1] + 1;

    while (primes_len < (size_t)n_needed) {
        bool isprime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            idx_t p = primes[i];
            /* If p*p > candidate, we can stop testing */
            if (p > candidate / p)
                break;
            if (candidate % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                primes = tmp;
            }
            primes[primes_len++] = candidate;
        }
        ++candidate;
    }
}

/* OEIS sequence A230098 */
seq_t a(idx_t n)
{
    /* Ensure we have the n-th prime */
    ensure_prime_count(n);
    idx_t prime_n = primes[n - 1];

    /* Ensure we have the prime of that prime */
    ensure_prime_count(prime_n);
    idx_t prime_pp = primes[prime_n - 1];

    seq_t n_seq = (seq_t)n;
    seq_t pn_seq = (seq_t)prime_n;
    seq_t ppp_seq = (seq_t)prime_pp;

    return n_seq * ppp_seq - pn_seq * pn_seq;
}

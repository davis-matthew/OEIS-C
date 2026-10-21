/*
 * A209628  Number of squarefree numbers < n that are not prime.
 * https://oeis.org/A209628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A013928(n) - pi(n) where A013928(n) is the number of
 *            square‑free integers ≤ n and pi(n) is the number of primes ≤ n.
 *            This counts the square‑free numbers < n that are not prime (the
 *            sequence is indexed from 1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* static caches for sieve data */
static size_t cache_limit = 0;
static bool   *is_prime  = NULL;   /* is_prime[i] == true iff i is prime */
static unsigned long long *prime_cnt = NULL; /* pi[i] */
static signed char *mu_arr = NULL;          /* Möbius values μ(i) */

/* integer square‑root (floor) for idx_t values */
static idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    while (((idx_t)(r + 1) * (idx_t)(r + 1)) <= n)
        ++r;
    return r;
}

/* rebuild cache up to at least 'limit' (inclusive) */
static void ensure_cache(idx_t limit)
{
    if ((size_t)limit < cache_limit)
        return;

    /* Allocate new arrays */
    size_t sz = (size_t)limit + 1;
    bool *new_prime = (bool *)calloc(sz, sizeof(bool));
    if (!new_prime)
        return;
    unsigned long long *new_prime_cnt = (unsigned long long *)
                                        calloc(sz, sizeof(unsigned long long));
    if (!new_prime_cnt) {
        free(new_prime);
        return;
    }
    signed char *new_mu = (signed char *)malloc(sz * sizeof(signed char));
    if (!new_mu) {
        free(new_prime);
        free(new_prime_cnt);
        return;
    }

    /* Sieve of Eratosthenes */
    for (size_t i = 0; i < sz; ++i)
        new_prime[i] = true;
    new_prime[0] = new_prime[1] = false;
    for (size_t i = 2; i * i < sz; ++i)
        if (new_prime[i])
            for (size_t j = i * i; j < sz; j += i)
                new_prime[j] = false;

    /* Prefix prime counts pi(i) */
    unsigned long long count = 0;
    for (size_t i = 0; i < sz; ++i) {
        if (i >= 2 && new_prime[i])
            ++count;
        new_prime_cnt[i] = count;
    }

    /* Möbius μ(i) */
    new_mu[0] = 0;
    if (sz > 1)
        new_mu[1] = 1;
    for (size_t i = 2; i < sz; ++i)
        new_mu[i] = 1;

    for (size_t p = 2; p < sz; ++p) {
        if (new_prime[p]) {
            for (size_t j = p; j < sz; j += p)
                if (new_mu[j] != 0)
                    new_mu[j] = (signed char)(-new_mu[j]);
            for (unsigned long long j = (unsigned long long)p * p;
                 j < sz; j += (unsigned long long)p * p)
                new_mu[j] = 0;
        }
    }

    /* Replace old cache */
    free(is_prime);
    free(prime_cnt);
    free(mu_arr);
    is_prime  = new_prime;
    prime_cnt = new_prime_cnt;
    mu_arr    = new_mu;
    cache_limit = sz - 1;
}

/* Main function for the sequence */
seq_t a(idx_t n)
{
    if (n == 0)          /* out of sequence range */
        return 0;

    /* Ensure sieve data is prepared up to n */
    ensure_cache(n);

    /* Count square‑free numbers ≤ n via Möbius inversion */
    idx_t root = int_sqrt(n);
    long long sq_free = 0;
    for (idx_t d = 1; d <= root; ++d) {
        int mu_d = mu_arr[d];
        if (mu_d == 0)
            continue;
        idx_t div = (idx_t)d * (idx_t)d;
        sq_free += (long long)mu_d * (long long)(n / div);
    }

    /* Number of primes ≤ n */
    unsigned long long primes = prime_cnt[n];

    long long result = sq_free - (long long)primes;
    return (seq_t)result;
}

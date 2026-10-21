/*
 * A097457  If n is prime replace with the n-th nonprime otherwise replace
 *          with the n-th prime.
 * https://oeis.org/A097457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A097457 – if n is prime replace with the n-th nonprime otherwise
 *            replace with the n-th prime
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint32_t *g_primes = NULL;
static size_t g_prime_count = 0;
static uint32_t *g_pi     = NULL;   /* cumulative prime counts: g_pi[i] = #primes <= i */
static size_t g_limit = 0;
static uint8_t *g_sieve = NULL;

/* Expand the sieve to at least "limit" (inclusive). */
static void ensure_sieve_size(size_t limit)
{
    if (limit <= g_limit) return;          /* already big enough */

    size_t new_limit = limit;
    size_t new_size  = new_limit + 1;

    uint8_t *new_sieve = (uint8_t*)calloc(new_size, sizeof(uint8_t));
    if (!new_sieve) return;                /* allocation failed – rely on caller */

    /* rebuild sieve completely for simplicity */
    for (size_t i = 2; i * i <= new_limit; ++i)
        if (!new_sieve[i])
            for (size_t j = i * i; j <= new_limit; j += i)
                new_sieve[j] = 1;           /* mark composite */

    /* free old data */
    free(g_sieve);
    g_sieve = new_sieve;

    /* build primes list and pi array */
    if (g_primes) free(g_primes);
    if (g_pi)     free(g_pi);

    size_t primes_est = (size_t)(new_limit / log((double)new_limit) * 1.2) + 10;
    g_primes = (uint32_t*)malloc(primes_est * sizeof(uint32_t));
    g_pi     = (uint32_t*)malloc(new_size * sizeof(uint32_t));

    if (!g_primes || !g_pi) return; /* allocation failed */

    size_t cnt = 0;
    g_pi[0] = 0;
    for (size_t i = 1; i <= new_limit; ++i) {
        g_pi[i] = g_pi[i-1] + ( (i >= 2 && !g_sieve[i]) ? 1 : 0 );
        if (i >= 2 && !g_sieve[i]) {
            if (cnt < primes_est) g_primes[cnt++] = (uint32_t)i;
        }
    }
    g_prime_count = cnt;
    g_limit = new_limit;
}

/* Return the n-th prime (1‑based). Builds sieve if needed. */
static seq_t nth_prime(idx_t n)
{
    if (n == 0) return 0;

    /* ensure we have at least n primes */
    while (g_prime_count < (size_t)n) {
        size_t new_limit;
        if (n < 6) new_limit = 15;
        else new_limit = (size_t)(n * (log((double)n) + log(log((double)n))) + 10);
        if (new_limit <= g_limit) new_limit = g_limit * 2 + 100;
        ensure_sieve_size(new_limit);
    }
    return (seq_t)g_primes[n-1];
}

/* Return the n-th nonprime (1‑based), where nonprimes are 1,4,6,8,9,… */
static seq_t nth_nonprime(idx_t n)
{
    if (n == 0) return 0;
    /* we need to find smallest x with x - pi[x] >= n */
    size_t low = 1, high = g_limit;
    /* extend limit until condition holds */
    while ((high - g_pi[high]) < n) {
        size_t new_limit = g_limit * 2 + 100;
        ensure_sieve_size(new_limit);
        high = g_limit;
    }
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if ((mid - g_pi[mid]) >= n) high = mid;
        else low = mid + 1;
    }
    return (seq_t)low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* ensure at least n primes and enough limit for composites */
    size_t needed_limit;
    if (n < 6) needed_limit = 15;
    else needed_limit = (size_t)(n * (log((double)n) + log(log((double)n))) + 10);
    if (needed_limit < 100) needed_limit = 100;
    if (needed_limit > g_limit)
        ensure_sieve_size(needed_limit);

    /* decide primality of n using small prime test over precomputed primes */
    bool n_is_prime;
    if (n < 2) n_is_prime = false;
    else if (n <= g_limit) n_is_prime = (!g_sieve[(size_t)n]);
    else {
        /* fallback simple trial division up to sqrt(n) */
        n_is_prime = true;
        for (size_t i = 0; i < g_prime_count && (uint64_t)g_primes[i]*(uint64_t)g_primes[i] <= n; ++i)
            if (n % g_primes[i] == 0) { n_is_prime = false; break; }
    }

    if (n_is_prime) return nth_nonprime(n);
    else            return nth_prime(n);
}

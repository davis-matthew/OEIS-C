/*
 * A087461  Arithmetic mean of n-th and 2n-th primes.
 * https://oeis.org/A087461
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n + p_2n)/2 (p_k = k-th prime)
 * Range:     u8 n <= 27, u16 n <= 4322, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Cache of primes: prime_cache[0] = 2, prime_cache[1] = 3, ... */
static idx_t *prime_cache = NULL;
static size_t prime_count = 0;

/* Ensure at least 'need' primes are in prime_cache. */
static void ensure_primes(idx_t need) {
    if (prime_count >= (size_t)need)
        return;                       /* already enough */

    /* Est. upper bound for the needed prime (Rosser's theorem). */
    double n = (double)need;
    double upper;
    if (need < 6)
        upper = 15.0;
    else
        upper = n * (log(n) + log(log(n))) + 10.0;
    idx_t limit = (idx_t)upper;

    /* Sieve of Eratosthenes up to 'limit'. */
    char *sieve = (char *)malloc((size_t)limit + 1);
    if (!sieve) abort();
    memset(sieve, 1, (size_t)limit + 1);
    if (limit >= 0) sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;
    for (size_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes to resize the cache. */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++count;

    /* Reallocate cache and fill it. */
    idx_t *new_cache = (idx_t *)realloc(prime_cache, count * sizeof(idx_t));
    if (!new_cache) abort();
    prime_cache = new_cache;
    prime_count = count;

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) prime_cache[pos++] = i;

    free(sieve);
}

/* Sequence function. */
seq_t a(idx_t n) {
    if (n < 2) return 0;                    /* values below offset irrelevant */

    /* Need primes up to index 2n. */
    ensure_primes(2 * n);

    seq_t p_n  = (seq_t)prime_cache[(size_t)(n - 1)];   /* nth prime    */
    seq_t p_2n = (seq_t)prime_cache[(size_t)(2 * n - 1)];/* 2n-th prime */

    return (p_n + p_2n) / 2;                /* sum is even → integer division exact */
}

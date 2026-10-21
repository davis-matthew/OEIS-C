/*
 * A096139  Number of ways to write 2*n as an ordered sum of two numbers which
 *          are prime or 1.
 * https://oeis.org/A096139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of ordered pairs (p,q) such that p+q = 2*n and p,q
 *            ∈ {1} ∪ primes(>1) (OEIS A096139)
 * Range:     u8 n <= 1364, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for prime sieve and memoization */
static bool *is_prime = NULL;        /* sieve of size sieve_len   */
static size_t sieve_len = 0;         /* current sieve size        */
static seq_t *cache = NULL;          /* memoization for a(n)      */
static size_t cache_len = 0;         /* number of cached terms     */

/* update sieve to at least 'limit' (inclusive) */
static void ensure_sieve(size_t limit)
{
    if (limit < 2) return;
    if (sieve_len >= limit + 1) return;          /* already enough */

    size_t new_len = limit + 1;
    bool *new_is_prime = (bool*)calloc(new_len, 1);
    if (!new_is_prime) return;                  /* allocation failed */

    /* simple sieve */
    new_is_prime[0] = new_is_prime[1] = false;
    for (size_t i = 2; i < new_len; ++i) new_is_prime[i] = true;
    for (size_t i = 2; i * i < new_len; ++i)
        if (new_is_prime[i])
            for (size_t j = i * i; j < new_len; j += i)
                new_is_prime[j] = false;

    /* free old sieve */
    free(is_prime);
    is_prime = new_is_prime;
    sieve_len = new_len;
}

/* ensure memoization array can hold index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    if (cache_len >= n) return;      /* already enough */

    size_t new_len = n * 2;          /* grow by factor 2 to avoid many reallocs */
    seq_t *new_cache = (seq_t*)calloc(new_len, sizeof(seq_t));
    if (!new_cache) return;          /* allocation failed */
    /* copy old values */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1; return 0 */

    /* memoization check */
    ensure_cache(n);
    if (cache[n-1] != 0) return cache[n-1];   /* 0 is sentinel (terms never 0) */

    uint64_t target = 2ULL * n;
    ensure_sieve((size_t)target);

    seq_t count = 0;
    for (size_t p = 1; p < target; ++p) {
        if (p == 1 || is_prime[p]) {
            size_t q = target - p;
            if (q == 1 || is_prime[q]) {
                ++count;
            }
        }
    }

    cache[n-1] = count;
    return count;
}

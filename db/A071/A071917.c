/*
 * A071917  Number of pairs (x,y) where x is even, y is odd, 1<=x<=n, 1<=y<=n
 *          and x+y is prime.
 * https://oeis.org/A071917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 946, u32 n <= 8675, u64 n <= 8675,
 *            u128 n <= 8675, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A071917
   Formula: a(n) = a(n-1) + pi(2*n-1) - pi(n),  a(1) = 0
   (pi(k) = number of primes <= k) */

#include <stdlib.h>   /* malloc, calloc, realloc, free, NULL */
#include <stdint.h>   /* uint8_t, uint32_t */
#include <stdbool.h>  /* bool */

static idx_t    sieve_limit   = 0;    /* maximum n for which pi table is valid */
static uint32_t *pi_prefix  = NULL;   /* pi_prefix[i] = pi(i)  (0 <= i <= sieve_limit) */

static idx_t   cache_limit   = 0;      /* maximum n for which a_cache computed */
static seq_t * a_cache       = NULL;   /* a_cache[n] = a(n) for 0 <= n <= cache_limit */

/* ------------------------------------------------------------------ */
/* recompute primes and pi_prefix up to |limit| (inclusive)           */
/* ------------------------------------------------------------------ */
static void recompute_sieve(idx_t limit)
{
    /* allocate boolean flags temporarily */
    size_t sz = (size_t)(limit + 1);
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime) return;         /* memory failure – abort silently */

    /* sieve of Eratosthenes */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* compute pi_prefix */
    free(pi_prefix);
    pi_prefix = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!pi_prefix) { free(is_prime); return; }

    pi_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi_prefix[i] = pi_prefix[i-1] + (is_prime[i] ? 1 : 0);

    free(is_prime);
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* extend a_cache up to |max_n|                                         */
/* ------------------------------------------------------------------ */
static void extend_cache(idx_t max_n)
{
    if (max_n <= cache_limit) return;

    size_t new_sz = (size_t)(max_n + 1);
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_sz * sizeof(seq_t));
    if (!new_cache) return;          /* memory failure – abort silently */
    a_cache = new_cache;

    /* initialize first element if needed */
    if (cache_limit == 0) a_cache[0] = (seq_t)0;
    if (cache_limit == 0) a_cache[1] = (seq_t)0;

    /* compute new entries */
    for (idx_t i = cache_limit + 1; i <= max_n; ++i) {
        idx_t two_minus_one = 2 * i - 1;
        seq_t diff = (seq_t)(pi_prefix[two_minus_one] - pi_prefix[i]);
        a_cache[i] = a_cache[i-1] + diff;
    }
    cache_limit = max_n;
}

/* ------------------------------------------------------------------ */
/* Main function: a(n)  (public API)                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined below offset, return 0 */

    /* ensure prime table up to 2*n-1 */
    idx_t max_limit = 2 * n - 1;
    if (sieve_limit < max_limit) recompute_sieve(max_limit);

    /* ensure cache up to n */
    if (cache_limit < n) extend_cache(n);

    return a_cache[n];
}

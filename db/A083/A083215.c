/*
 * A083215  a(n) = 1 + Sum(prime(i)*(2*i-1): 1<=i<=n).
 * https://oeis.org/A083215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{i=1..n} prime(i)*(2*i-1) (OEIS A083215)
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
 *  Prime generation – we keep a list of primes and extend it
 *  lazily when more primes are needed.
 * ------------------------------------------------------------------ */

static idx_t       primes_count   = 0;   /* number of primes stored   */
static size_t      primes_cap     = 0;   /* capacity of primes array */
static idx_t      *primes         = NULL;/* array of primes           */

/* Extend primes array capacity when necessary */
static void primes_grow(void)
{
    size_t new_cap = primes_cap ? primes_cap * 2 : 8;
    idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    if (!tmp) abort();                 /* cannot recover from failure */
    primes = tmp;
    primes_cap = new_cap;
}

/* Append a new prime to the list */
static void primes_append(idx_t p)
{
    if (primes_count == primes_cap) primes_grow();
    primes[primes_count++] = p;
}

/* Test whether a number is prime using primes already found
   (all primes up to sqrt(candidate) are guaranteed to be in the list) */
static bool is_prime_candidate(idx_t candidate)
{
    for (idx_t i = 0; i < primes_count; ++i)
    {
        idx_t p = primes[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Generate primes until we have at least n primes (1‑based index). */
static void generate_primes_upto(idx_t n)
{
    if (primes_count == 0)            /* initialise with 2 */
    {
        primes_grow();
        primes_append(2);
    }

    idx_t candidate = primes[primes_count - 1] + 1;
    if (candidate % 2 == 0) candidate++; /* skip even numbers */

    while (primes_count < n)
    {
        if (is_prime_candidate(candidate))
            primes_append(candidate);
        candidate += 2;                /* still skip even numbers */
    }
}

/* ------------------------------------------------------------------
 *  Sequence cache – we store computed values of a(n) so that
 *  repeated calls to a() in any order are O(1) after the first
 *  computation of the largest n requested.
 * ------------------------------------------------------------------ */

static seq_t      *cache          = NULL;   /* cache[i] holds a(i) */
static size_t      cache_cap      = 0;      /* allocated capacity      */
static idx_t       cache_max_computed = 0; /* highest n for which
                                            cache[n] is valid          */

/* Ensure the cache can hold indices up to n (inclusive). */
static void cache_ensure_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the base value a(0) = 1.
       The array is never accessed at index 0 for n≥1, but this
       simplifies the recurrence. */
    if (cache_max_computed == 0)
        cache[0] = (seq_t)1;
}

/* ------------------------------------------------------------------
 *  The sequence function
 * ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)            /* sequence offset is 1, but return 1 for n==0 */
        return (seq_t)1;

    /* Make sure we have enough primes. */
    generate_primes_upto(n);

    /* Make sure the cache can hold the new value. */
    cache_ensure_capacity(n);

    /* Compute any missing entries in the cache. */
    for (idx_t i = cache_max_computed + 1; i <= n; ++i)
    {
        /* term = prime(i) * (2*i - 1) */
        seq_t term = (seq_t)primes[i - 1] *
                     (((seq_t)2 * (seq_t)i) - 1);
        cache[i] = cache[i - 1] + term;   /* cache[0] = 1 ensures the '+1' */
    }
    cache_max_computed = n;

    return cache[n];
}

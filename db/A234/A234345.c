/*
 * A234345  Smallest q such that n <= q < 2n with p, q both prime, p+q = 2n,
 *          and p <= q.
 * https://oeis.org/A234345
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 237, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A234345:  the smallest q such that
   n <= q < 2n, both q and (2n−q) are prime,
   and q >= 2n−q.  The value returned is the larger part
   of the Goldbach partition of 2n having the smallest prime difference. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* prime sieve -----------------------------------------------------------------
   prime_flag[i] == 1  iff i is prime.
   prime_cap is the number of entries currently allocated (0..prime_cap-1). */
static char *prime_flag = NULL;
static size_t prime_cap = 0;

/* Ensure the sieve covers the range [0,limit] (inclusive). */
static void ensure_sieve(size_t limit)
{
    if (limit + 1 <= prime_cap)
        return;                      /* already sufficient */

    /* Allocate a fresh flag array. */
    size_t new_cap = limit + 1;
    char *tmp = (char *)malloc(new_cap);
    if (!tmp)
        abort();                     /* out of memory: stop evaluation */

    /* Initialise all to 'prime'. */
    for (size_t i = 0; i < new_cap; ++i)
        tmp[i] = 1;
    tmp[0] = tmp[1] = 0;              /* 0 and 1 are not prime */

    /* Eratosthenes sieve. */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (tmp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                tmp[j] = 0;
        }
    }

    /* Replace old sieve. */
    free(prime_flag);
    prime_flag = tmp;
    prime_cap  = new_cap;
}

/* cache for a(n) ----------------------------------------------------------------
   cache[n] contains a(n) for n in [0,cache_cap-1].
   last_computed is the largest n for which a(n) is known. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  last_computed = 1;     /* values below the offset are undefined */

/* Grow the cache array to hold indices up to at least new_cap-1. */
static void expand_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();                     /* out of memory */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for all n in (last_computed, new_max]. */
static void compute_up_to(idx_t new_max)
{
    if (new_max <= last_computed)
        return;

    /* Ensure sieve covers the entire range of numbers we will test. */
    size_t sieve_limit = (size_t)(2ULL * (uint64_t)new_max);
    ensure_sieve(sieve_limit);

    /* Ensure cache can hold all indices up to new_max. */
    size_t needed = (size_t)(new_max + 1);
    expand_cache(needed);

    /* Compute the missing values. */
    for (idx_t n = last_computed + 1; n <= new_max; ++n) {
        size_t two_n = 2 * (size_t)n;
        seq_t result = 0;

        /* Search for the smallest q in [n, 2n-1] with prime pair. */
        for (size_t q = (size_t)n; q < two_n; ++q) {
            size_t p = two_n - q;
            if (prime_flag[q] && prime_flag[p]) {
                result = (seq_t)q;
                break;
            }
        }
        cache[(size_t)n] = result;
    }

    last_computed = new_max;
}

/* required function ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2)            /* undefined below the offset */
        return 0;

    compute_up_to(n);
    return cache[(size_t)n];
}

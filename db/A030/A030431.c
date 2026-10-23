/*
 * A030431  Primes of form 10n+3.
 * https://oeis.org/A030431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1646, u32 n <= 9874, u64 n <= 9874,
 *            u128 n <= 9874, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A030431: primes of the form 10n+3 (n ≥ 1).  This implementation searches
 * candidates of the form 10k+3 and tests them for primality. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

static seq_t *prime_cache = NULL;   /* array of primes ≡ 3 (mod 10) */
static size_t  cache_size = 0;      /* number of stored primes          */
static size_t  cache_cap   = 0;      /* current capacity of the array    */
static uint64_t last_candidate = 3;  /* last candidate tested (3 initially) */

/* Ensure the cache can hold 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        if (cache_cap < need) cache_cap = need;
        prime_cache = seq_realloc(prime_cache, cache_size, cache_cap);
    }
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* undefined, safe guard */

    /* If the cache already has the desired term, return it. */
    if ((idx_t)cache_size >= n)
        return prime_cache[n - 1];

    /* Initialize the cache with the first prime 3, if not already done. */
    if (cache_size == 0) {
        ensure_capacity(1);
        prime_cache[0] = (seq_t)3;
        cache_size = 1;
        last_candidate = 3;
    }

    /* Generate further primes until we reach the desired index. */
    while ((idx_t)cache_size < n) {
        last_candidate += 10;                     /* next candidate of form 10k+3 */
        if (isPrimeGeneric((seq_t)last_candidate)) {
            ensure_capacity(cache_size + 1);
            prime_cache[cache_size++] = (seq_t)last_candidate;
        }
    }

    return prime_cache[n - 1];
}

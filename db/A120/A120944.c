/*
 * A120944  Composite squarefree numbers.
 * https://oeis.org/A120944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120944: composite squarefree numbers computed by sieving for
   smallest prime factors and linear search. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for sequence terms */
static seq_t *cache      = NULL;  /* array of a(1) .. a(cache_len)          */
static size_t cache_len  = 0;     /* number of terms currently stored      */
static size_t cache_cap  = 0;     /* allocated capacity of cache           */

/* Smallest prime factor table */
static uint64_t *spf      = NULL; /* spf[i] = smallest prime divisor of i  */
static idx_t    spf_limit = 0;    /* current maximum index covered        */

/* Next candidate to test for being composite squarefree */
static idx_t    next_candidate = 4;   /* start from 4 (2×2 is not squarefree) */

/* Ensure the cache array has capacity at least 'min_cap' */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap < 8 ? 8 : cache_cap;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);   /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Recompute the smallest–prime‑factor table up to at least 'limit' */
static void recompute_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    free(spf);
    spf = (uint64_t *)calloc((size_t)(limit + 1), sizeof(uint64_t));
    if (!spf) exit(EXIT_FAILURE);        /* out of memory */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;                   /* i is prime */
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* Test whether x is squarefree using the spf table */
static bool is_squarefree(idx_t x)
{
    idx_t m = x;
    while (m > 1) {
        idx_t p = spf[m];
        m /= p;
        if (m % p == 0) return false;    /* repeated prime factor */
    }
    return true;
}

/* Test whether x is composite (non‑prime, >1) */
static bool is_composite(idx_t x)
{
    return x > 1 && spf[x] != x;
}

/* Return the n-th composite squarefree number (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* Grow cache to hold at least n terms */
    grow_cache((size_t)n);

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Ensure spf table covers the current candidate */
        if (next_candidate > spf_limit) {
            idx_t new_limit = spf_limit ? (spf_limit << 1) : (idx_t)64;
            while (new_limit < next_candidate) new_limit <<= 1;
            recompute_spf(new_limit);
        }

        /* Test the current candidate */
        if (is_composite(next_candidate) && is_squarefree(next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];   /* offset 1 */
}

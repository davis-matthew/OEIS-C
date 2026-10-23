/*
 * A115662  Nonpalindromic k which have the same number of distinct prime
 *          divisors as the reverse of k.
 * https://oeis.org/A115662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns a(n) for OEIS A115662: the n-th
   non‑palindromic integer k such that the number of distinct
   prime divisors of k equals that of its decimal reverse. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/rev.h"
#include "../utils/count_distinct_primes.h"

static seq_t *cache = NULL;          /* cached sequence terms */
static size_t cache_size = 0;        /* number of terms currently stored */
static size_t cache_cap   = 0;        /* allocated capacity (in seq_t elements) */
static uint64_t search_start = 1;    /* last integer examined in the search */

/* Expand the cache until at least 'n' terms are available. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_size)
        return;
    while ((size_t)n > cache_size)
    {
        /* Look for the next qualifying integer. */
        search_start++;                     /* start checking from previous+1 */
        uint64_t k      = search_start;
        uint64_t revk   = rev(k);
        if (revk == k)                      /* skip palindromes */
            continue;

        if (count_distinct_primes(k) == count_distinct_primes(revk))
        {
            /* Grow storage if necessary. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* unrecoverable allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the term.  Cast from uint64_t works both in C (simple
               integer conversions) and in C++ (calls the appropriate
               constructor if seq_t is a class). */
            cache[cache_size++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based according to OEIS offset. */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}

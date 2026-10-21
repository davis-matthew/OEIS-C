/*
 * A079712  Numbers m such that bigomega(m) = 3*omega(m).
 * https://oeis.org/A079712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 654, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   bigomega, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: sequential search using bigomega and count_distinct_primes. */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"

/* Static cache for the sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Largest integer already tested (0 means none yet) */
static uint64_t last_checked = 0;

/* Ensure the cache can hold at least 'need' entries. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_len >= need)
        return;                       /* already sufficient */

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate the next term that satisfies bigomega(m) = 3*omega(m). */
static void generate_next_term(void)
{
    for (;;)
    {
        ++last_checked;                     /* candidate integer */

        /* Compute the total and distinct prime factor counts */
        idx_t cand = (idx_t)last_checked;
        seq_t  bigo   = bigomega(cand);              /* Ω(m) */
        uint64_t distinct = count_distinct_primes(last_checked); /* ω(m) */

        /* Check defining condition */
        if (bigo == 3 * (seq_t)distinct)
        {
            /* Ensure enough capacity */
            ensure_cache_capacity(cache_len + 1);

            /* Store the term */
            cache[cache_len++] = (seq_t)last_checked;
            break;                            /* one term found */
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for offset < 1 */

    /* Generate terms until we have at least n terms */
    while (cache_len < (size_t)n)
        generate_next_term();

    return cache[n - 1];
}

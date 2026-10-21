/*
 * A077680  Squarefree numbers beginning with 4.
 * https://oeis.org/A077680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6768, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree, starts_with_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077680: a(n) = the nth squarefree integer whose first decimal digit is 4 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/starts_with_4.h"

/* Simple cache of terms that have already been found */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Next integer to test in the search */
static idx_t next_candidate = 41;   /* first positive integer >4 that starts with 4 */

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; behaviour for n=0 is unused */

    /* Initialise cache on the first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(EXIT_FAILURE);   /* out of memory */
        cache_len = 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* advance to the next integer that both starts with 4 and is squarefree */
        while (!starts_with_4(next_candidate) ||
               !is_squarefree((seq_t)next_candidate))
            ++next_candidate;

        /* store found term, growing the cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
            cache = tmp;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;   /* move to the next candidate for the following iteration */
    }

    return cache[n - 1];
}

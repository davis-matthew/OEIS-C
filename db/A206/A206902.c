/*
 * A206902  Number of nonisomorphic graded posets with 0 and uniform Hasse
 *          diagram of rank n with no 3-element antichain.
 * https://oeis.org/A206902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) + 3*a(n-3), a(0)=1, a(1)=2, a(2)=8,
 *            a(3)=36
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;    /* array of computed terms */
static size_t cache_len  = 0;       /* current allocated length      */
static size_t cache_upto = 0;       /* highest index that is filled   */

/* Ensure that cache holds values up to index n. */
static void ensure_cache(idx_t n)
{
    /* initialise first four terms */
    if (cache == NULL) {
        size_t init = 4;                     /* we need at least 4 slots   */
        cache = (seq_t *)malloc(sizeof(seq_t) * init);
        if (!cache) return;                  /* allocation failure unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)36;
        cache_len = init;
        cache_upto = 3;
    }

    /* if already computed, nothing to do */
    if ((size_t)n <= cache_upto) return;

    /* grow the allocated array if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = cache_len;
        while ((size_t)n + 1 > new_len) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp) cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_upto + 1; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)6 * cache[i - 1])
                 - ((seq_t)7 * cache[i - 2])
                 + ((seq_t)3 * cache[i - 3]);
    }
    cache_upto = (size_t)n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

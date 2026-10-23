/*
 * A008891  Aliquot sequence starting at 180.
 * https://oeis.org/A008891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n+1) = A001065(a(n)), a(0) = 180
 * Range:     u8 n <= 0, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001065(x) is the sum of proper divisors of x, computed as sigma_of(x) - x. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

/* Ensure that sz is sufficient for n+1 elements. */
static void ensure_capacity(idx_t n, seq_t **p, size_t *cap)
{
    size_t needed = (size_t)n + 1;
    if ((*cap) >= needed) return;
    size_t newcap = (*cap) ? (*cap) * 2 : 1;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(*p, newcap * sizeof(seq_t));
    if (!tmp) { /* allocation failed – abort (sequence cannot be computed) */
        abort();
    }
    *p = tmp;
    *cap = newcap;
}

/* Compute the sequence value a(n).  The implementation caches computed terms
 * so that repeated calls are fast and the order of calls is arbitrary. */
seq_t a(idx_t n)
{
    /* Static cache state */
    static seq_t *cache = NULL;    /* cache[0] .. cache[max_n] */
    static size_t cache_cap = 0;   /* allocated size of cache */
    static idx_t max_n = (idx_t)-1;/* last index that has been computed */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();             /* cannot proceed without memory */
        cache[0] = (seq_t)180;           /* initial value a(0) */
        max_n = 0;
    }

    /* If requested index already computed, return it */
    if (n <= max_n) {
        return cache[n];
    }

    /* Ensure cache is large enough to hold the new values */
    ensure_capacity(n, &cache, &cache_cap);

    /* Compute missing terms one by one */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Compute aliquot part: proper divisors sum = sigma_of(x) - x */
        seq_t prev = cache[i - 1];
        /* sigma_of takes idx_t; our values fit in idx_t for this sequence. */
        seq_t s = sigma_of((idx_t)prev);
        cache[i] = s - prev;
    }
    max_n = n;
    return cache[n];
}

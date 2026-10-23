/*
 * A175018  A005185(n)-floor(A005185(n)/2).
 * https://oeis.org/A175018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005185(n)-floor(A005185(n)/2) = ceil(A005185(n)/2) =
 *            (A005185(n)+1)/2 (OEIS A175018)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Cache for Hofstadter Q-function values (A005185).  The array is
   0‑based but we only use indices >=1; cache[0] is unused. */
static seq_t *cache = NULL;      /* pointer to the cached values */
static idx_t  cache_len = 0;     /* highest index that has been computed */
static idx_t  cache_cap = 0;     /* current capacity-1 (max usable index) */

/* Ensure the cache can hold indices up to ‘n’. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;                     /* already sufficient */

    /* Allocate a new array large enough for indices 0 .. n. */
    idx_t new_cap = n;               /* highest usable index after reallocation */
    seq_t *new_buf = alloc_seq_array((size_t)(new_cap + 1)); /* 0..new_cap */

    /* If an old buffer exists, copy its contents into the new one. */
    if (cache != NULL) {
        for (idx_t i = 0; i <= cache_len; ++i)
            new_buf[i] = cache[i];
        free_seq_array(cache);
    }

    cache      = new_buf;
    cache_cap  = new_cap;            /* maximum index that can be accessed */
}

/* Compute a(n) = ceil(A005185(n)/2).  n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;             /* undefined for n < 1 */

    ensure_cache(n);

    /* Compute missing Hofstadter Q values up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            cache[i] = (seq_t)1;
        } else {
            idx_t idx1 = i - (idx_t)cache[i - 1];
            idx_t idx2 = i - (idx_t)cache[i - 2];
            cache[i]  = cache[idx1] + cache[idx2];
        }
    }
    cache_len = n;                   /* highest computed index */

    /* a(n) = (Q(n)+1)/2.  Q(n) is cache[n]. */
    return (cache[n] + (seq_t)1) / (seq_t)2;
}

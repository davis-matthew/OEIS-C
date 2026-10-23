/*
 * A128021  a(n) = A128020(n)/n.
 * https://oeis.org/A128021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128021
   a(n) = 1 + floor( (∑_{k=1}^{n-1} k·a(k)) / n )
   Initial value: a(1)=1
   (Same recurrence as described in the OEIS entry.) */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* local cache of already computed terms (1‑based) */
static seq_t *cache       = NULL;     /* array of a(i) */
static size_t cache_cap   = 0;        /* allocated size of cache (>= i+1 for any i) */
static idx_t  max_computed = 0;       /* largest n for which cache[1..max_computed] are valid */

/* running sum S(n)=∑_{k=1}^{n} k·a(k)  (always up to current max_computed) */
static seq_t sum_total = 0;

/* Ensure the cache array can address index n (plus 1 because index 0 is unused). */
static void ensure_cache(idx_t n)
{
    /* need capacity ≥ n+1 */
    size_t required = (size_t)n + 1;
    if (cache_cap >= required)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2u : 16u;
    while (new_cap < required)
        new_cap *= 2u;

    seq_t *tmp;
    if (cache_cap) {
        tmp = seq_realloc(cache, cache_cap, new_cap);
    } else {
        tmp = seq_alloc(new_cap);
    }
    if (!tmp) abort();          /* memory allocation failed */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Return a(n) for n ≥ 1.  Subsequent calls may be in any order
   and the function remains O(1) for already computed n. */
seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for n < 1 */
        return 0;

    /* make sure our cache can hold index n */
    ensure_cache(n);

    /* perform one‑time initialization if there's no data yet */
    if (max_computed == 0) {
        cache[1]   = (seq_t)1;
        sum_total  = (seq_t)1;  /* 1·a(1) */
        max_computed = 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 1 + floor( sum_total / i ) */
        seq_t ai = (seq_t)1 + (sum_total / (seq_t)i);
        cache[i] = ai;
        /* update running sum: S(i) = S(i−1) + i·a(i) */
        sum_total += (seq_t)i * ai;
    }
    max_computed = n;
    return cache[n];
}

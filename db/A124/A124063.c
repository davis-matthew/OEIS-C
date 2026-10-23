/*
 * A124063  a(1)=a(2)=1. a(n) = the sum of the terms, from among the first
 *          (n-2) terms of the sequence, which divide a(n-1).
 * https://oeis.org/A124063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=1.
   For n≥3, a(n) = sum_{i=1}^{n-2} a(i)  if a(i) | a(n-1).   (OEIS A124063) */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for already computed terms.  cache[0] = a(1), etc. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* capacity of the cache */

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Initialise cache with a(1)=a(2)=1. */
    if (cache_len == 0) {
        ensure_cache_cap(2);
        cache[0] = 1;   /* a(1) */
        cache[1] = 1;   /* a(2) */
        cache_len = 2;
    }

    if (n <= cache_len)          /* cached value exists */
        return cache[n - 1];

    /* Compute missing terms up to n.  We use a growing cache. */
    for (idx_t cur = cache_len + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        if (cur >= 3) {
            seq_t prev = cache[cur - 2];          /* a(cur-1) */
            size_t limit = (size_t)(cur - 2);     /* number of terms to examine */
            for (size_t i = 0; i < limit; ++i) {
                if (prev % cache[i] == 0)
                    sum += cache[i];
            }
        } else {
            sum = 1;   /* for cur == 1 or cur == 2, but loop never reaches here */
        }
        /* Store the computed value. */
        if (cache_len >= cache_cap)
            ensure_cache_cap(cache_len + 1);
        cache[cache_len] = sum;   /* cache index == cur-1 */
        cache_len++;
    }

    return cache[n - 1];
}

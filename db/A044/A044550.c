/*
 * A044550  Numbers n such that string 4,3 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has43
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A044550: numbers n for which the base‑7 representation of n
   contains the consecutive digits 4,3 but the base‑7 representation of n+1
   does not.  A simple search with caching is used. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has43.h"

static seq_t *cache = NULL;           /* stored terms          */
static size_t cache_cap = 0;          /* allocated space       */
static size_t cache_len = 0;          /* number of terms stored */
static seq_t last_checked = (seq_t)0; /* largest n already tested */

/* Increase cache capacity to at least 'needed' entries. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Generate terms until the cache contains at least 'n' entries. */
static void generate_until(idx_t n)
{
    while (cache_len < n) {
        seq_t candidate = last_checked + (seq_t)1;
        last_checked = candidate;

        if (!has43(candidate))
            continue;
        seq_t next = candidate + (seq_t)1;
        if (has43(next))
            continue;

        grow_cache(cache_len + 1);
        cache[cache_len++] = candidate;
    }
}

/* OEIS: a(n) = n-th term of A044550, 1‑based index. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    generate_until(n);
    return cache[n - 1];
}

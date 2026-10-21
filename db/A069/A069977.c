/*
 * A069977  Numbers k such that k and k+2 are squarefree.
 * https://oeis.org/A069977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A069977: numbers k such that k and k+2 are squarefree. */

#include <stdlib.h>
#include "../utils/is_squarefree.h"

/* Static cache for the sequence values */
static seq_t *cached_vals   = NULL;   /* array of cached values      */
static size_t cached_len    = 0;      /* number of cached values     */
static size_t cached_cap    = 0;      /* allocated capacity          */
static idx_t last_checked   = 0;      /* last k that was examined    */

/* Ensure the cache can hold at least `need` elements. */
static void grow_cache(size_t need)
{
    if (cached_cap >= need) {               /* already big enough          */
        return;
    }
    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
    if (new_cap < need) {
        new_cap = need;
    }
    /* Cast the void* returned by realloc to the correct pointer type. */
    seq_t *tmp = (seq_t *)realloc(cached_vals, new_cap * sizeof(seq_t));
    if (!tmp) {                               /* fallback on out of memory   */
        abort();
    }
    cached_vals = tmp;
    cached_cap  = new_cap;
}

/* Append a value to the cache, growing it if necessary. */
static void cache_append(seq_t v)
{
    if (cached_len == cached_cap) {
        grow_cache(cached_cap + 1);
    }
    cached_vals[cached_len++] = v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {                     /* offset 1; undefined for n<1 */
        return 0;
    }

    /* Expand the cache until it contains at least n terms. */
    while (cached_len < (size_t)n) {
        ++last_checked;                         /* next candidate k          */
        seq_t k  = (seq_t)last_checked;         /* k                         */
        seq_t k2 = (seq_t)last_checked + 2ULL;  /* k + 2                     */
        if (is_squarefree(k) && is_squarefree(k2)) {
            cache_append(k);
        }
    }

    return cached_vals[n - 1];
}

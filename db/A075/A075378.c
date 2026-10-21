/*
 * A075378  Rearrangement of natural numbers with a(1) = 1, a(2n) squarefree
 *          and a(2n+1) not squarefree.
 * https://oeis.org/A075378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥1, a(2n) = smallest unused square‑free integer,
 *            a(2n+1)= smallest unused non‑square‑free integer
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_squarefree.h"

static seq_t *cache = NULL;   /* memoized terms (index n maps to cache[n‑1]) */
static size_t cache_len = 0;  /* number of computed terms */
static size_t cache_cap = 0;  /* allocated capacity in cache */

/* Next candidates for unused square‑free and non‑square‑free numbers */
static seq_t sf_next = 2;     /* first unused square‑free number (>1) */
static seq_t nsf_next = 4;    /* first unused non‑square‑free number (>1) */

/* Ensure cache has capacity for at least `need` entries. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = new_arr;
    cache_cap = new_cap;
}

/* Initialize cache with the first term a(1) = 1. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = 1;          /* a(1) = 1 */
    cache_len = 1;
    sf_next = 2;
    nsf_next = 4;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n == 0)           /* behavior below offset is irrelevant */
        return 0;

    if ((size_t)n <= cache_len)
        return cache[n - 1];

    ensure_cache_cap((size_t)n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if ((i & 1) == 0) {          /* even position – square‑free */
            val = sf_next;
            do {
                ++sf_next;
            } while (!is_squarefree(sf_next));
        } else {                     /* odd position (>=3) – non‑square‑free */
            val = nsf_next;
            do {
                ++nsf_next;
            } while (is_squarefree(nsf_next));
        }
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}

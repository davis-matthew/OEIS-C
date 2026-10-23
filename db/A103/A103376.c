/*
 * A103376  a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = a(7) = a(8) = a(9) = 1
 *          and for n>9: a(n) = a(n-8) + a(n-9).
 * https://oeis.org/A103376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for 1 ≤ n ≤ 9; for n > 9, a(n) = a(n-8) + a(n-9)
 * Range:     u8 n <= 72, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* Cached values, 0‑indexed: a(1) -> cache[0] */
static idx_t cached_size = 0u;       /* Number of entries stored in cache */

/* Expand the cache to store at least 'need' entries (need ≥ cached_size). */
static void expand_cache(idx_t need)
{
    if (need <= cached_size)
        return;
    /* Ensure at least 10 elements for the base cases */
    if (need < 10u)
        need = 10u;
    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort the program (not expected in tests). */
        abort();
    }
    /* Initialize new entries */
    for (idx_t i = cached_size; i < need; ++i) {
        if (i < 9u) {
            new_cache[i] = (seq_t)1;
        } else {
            new_cache[i] = new_cache[i - 8u] + new_cache[i - 9u];
        }
    }
    cache = new_cache;
    cached_size = need;
}

seq_t a(idx_t n)
{
    if (n <= 0u)
        return (seq_t)0;   /* Undefined for 0, but return 0 to avoid UB */
    /* Ensure the cache covers up to index n */
    expand_cache(n);
    return cache[n - 1u];
}

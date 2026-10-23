/*
 * A037707  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0,2.
 * https://oeis.org/A037707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037707: Base‑9 digits are formed by the repeating pattern 1,3,0,2. */
/* Recurrence: a(0) = 0,
 * a(n) = 9*a(n-1) + digit(n)   (n ≥ 1),
 * where digit(n) = 1,3,0,2 for n = 1,2,3,4, repeat every four terms. */

#include <stddef.h>                 /* size_t */
#include "../utils/seq_alloc.h"    /* seq_t *seq_alloc(size_t); */
#include "../utils/seq_realloc.h"  /* seq_t *seq_realloc(seq_t*, size_t, size_t); */

static seq_t *cache = NULL;   /* Stores a(0)…a(k) */
static size_t cache_len = 0;  /* Number of values stored (0 … cache_len-1) */
static size_t cache_cap = 0;  /* Allocated size of cache */

/* Return the digit for position pos (1‑based). */
static inline seq_t digit (idx_t pos)
{
    /* pos-1 cycles through 0,1,2,3 → map to 1,3,0,2 */
    switch ((int)((pos - 1) % 4)) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)3;
        case 2: return (seq_t)0;
        default: return (seq_t)2;  /* pos % 4 == 0 → 2 */
    }
}

/* Ensure the cache contains values up to n (inclusive). */
static void ensure_cache (idx_t n)
{
    if ((size_t)n < cache_len) return;          /* already computed */

    /* Grow allocation if necessary. */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)n + 1) new_cap *= 2;

        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) return;          /* allocation failure: silently ignore */
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* Initialize cache[0] if this is the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Compute new values up to n. */
    const seq_t mult9 = (seq_t)9;
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = mult9 * cache[i - 1] + digit((idx_t)i);
    }
    cache_len = (size_t)n + 1;
}

seq_t a (idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of original range but safe */
    ensure_cache(n);
    return cache[(size_t)n];
}

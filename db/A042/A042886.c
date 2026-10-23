/*
 * A042886  Numerators of continued fraction convergents to sqrt(975).
 * https://oeis.org/A042886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042886: numerators of continued‑fraction convergents to sqrt(975)
 *
 * Recurrence (for n ≥ 8):
 *   a(n) = 2498 · a(n‑4) – a(n‑8)
 *
 * Initial values:
 *   a(0) = 31
 *   a(1) = 125
 *   a(2) = 281
 *   a(3) = 1249
 *   a(4) = 77719
 *   a(5) = 312125
 *   a(6) = 701969
 *   a(7) = 3120001
 *
 * The function is O(n) with memoisation and works for any call order.
 * It uses only standard C11 facilities (malloc, free, loops, arithmetic).
 */

#include <stdlib.h>

/* Forward declaration – the build provides seq_t and idx_t types. */

static seq_t *cache = NULL;     /* Cached values of the sequence.   */
static size_t cache_cap = 0;    /* Number of cache slots already allocated. */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Required number of elements.  We also ensure at least the first 8 terms
       are available because the recurrence needs indices up to n‑8. */
    idx_t need = n + 1;
    if (need < 8) need = 8;

    if ((idx_t)cache_cap >= need)          /* already enough storage           */
        return;

    /* Preserve the size that was already computed. */
    size_t old_cap = cache_cap;

    /* Allocate new storage. */
    size_t new_cap = (size_t)need;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();               /* allocation failed – terminate */

    /* Copy existing entries. */
    if (cache) {
        for (size_t i = 0; i < old_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache      = new_cache;
    cache_cap  = new_cap;

    /* Initialise the first eight base terms.  Existing ones are overwritten,
       but that does not affect correctness. */
    if (cache_cap >  0) cache[ 0] = (seq_t)31;
    if (cache_cap >  1) cache[ 1] = (seq_t)125;
    if (cache_cap >  2) cache[ 2] = (seq_t)281;
    if (cache_cap >  3) cache[ 3] = (seq_t)1249;
    if (cache_cap >  4) cache[ 4] = (seq_t)77719;
    if (cache_cap >  5) cache[ 5] = (seq_t)312125;
    if (cache_cap >  6) cache[ 6] = (seq_t)701969;
    if (cache_cap >  7) cache[ 7] = (seq_t)3120001;

    /* Compute any terms that are now required but were not previously known. */
    /* Note: we start from old_cap because indices < old_cap have already
       been computed. */
    for (idx_t k = old_cap; k <= n; ++k) {
        if (k < 8) continue;            /* base cases already set */
        /* a(k) = 2498 * a(k‑4) – a(k‑8) */
        cache[k] = (seq_t)2498 * cache[k-4] - cache[k-8];
    }
}

/* Return the n‑th term (with a(0) as the first term). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

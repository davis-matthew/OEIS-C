/*
 * A133683  Linear recurrence a(n) = a(n-3) + 2a(n-5), starting from all-one
 *          initial conditions.
 * https://oeis.org/A133683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-3) + 2*a(n-5), a(1) … a(5) = 1
 * Range:     u8 n <= 24, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq_array.h" /* provides static seq_t *alloc_seq_array(size_t) */

static seq_t *cache = NULL;   /* cache of computed terms, a(n) stored at cache[n-1] */
static size_t cap = 0;        /* allocated array size */
static idx_t cached = 0;      /* number of terms already stored (max index = cached) */

/* enlarge cache to hold [0 .. needed-1] indices */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;
    seq_t *new_cache = alloc_seq_array(needed);
    if (cache) {
        memcpy(new_cache, cache, sizeof(seq_t) * cached);
        free(cache);
    }
    cache = new_cache;
    cap = needed;
}

/* initialise the first five terms (all ones) if not already done */
static void init_base(void)
{
    if (cached >= 5)
        return;
    ensure_capacity(5);
    for (size_t i = 0; i < 5; ++i)
        cache[i] = 1;
    cached = 5;
}

/* compute a(n) with memoisation, works for any order of calls */
seq_t a(idx_t n)
{
    if (n <= 5)
        return 1;

    /* make space for all needed indices */
    ensure_capacity((size_t)n);
    init_base();

    /* compute missing terms up to n */
    while (cached < n) {
        idx_t idx = cached + 1;                     /* 1‑based index to compute */
        seq_t val = cache[idx - 3 - 1] + 2 * cache[idx - 5 - 1];
        cache[idx - 1] = val;
        ++cached;
    }
    return cache[n - 1];
}

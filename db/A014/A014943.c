/*
 * A014943  a(1)=1, a(n) = n*25^(n-1) + a(n-1).
 * https://oeis.org/A014943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 51, a(n) = n*25^(n-1) + a(n-1) (n ≥ 3)
 *            Recurrence algebraically equivalent: a(n) = 50*a(n-1) -
 *            625*a(n-2) + 1 (valid for n > 2). This implementation uses the
 *            linear‐recurrence version for fast, memory‑effective computation
 *            and memoises results so that calls in any order remain ef...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* array indexed from 1 */
static idx_t cache_cap = 0;      /* cached array size (number of elements) */

/* Ensure the cache can store values up to index n (inclusive). */
static void grow_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;          /* already large enough */

    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache) return;                  /* allocation failed – rely on caller */

    if (cache) {
        /* Copy existing values; works for built‑in and class types. */
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        /* Initialise the newly allocated tail to zero. */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            new_cache[i] = 0;
        free(cache);
    } else {
        /* First allocation – zero‑init the whole array. */
        memset(new_cache, 0, sizeof(seq_t) * new_cap);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1: value for n<1 is irrelevant */

    grow_cache(n);                  /* ensure cache big enough */

    /* Initialise the base cases lazily. */
    if (cache[1] == 0) cache[1] = 1;
    if (n >= 2 && cache[2] == 0) cache[2] = 51;

    /* Compute missing terms up to n using the linear recurrence. */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] == 0) {
            /* a(i) = 50*a(i-1) - 625*a(i-2) + 1 */
            cache[i] = cache[i-1] * 50 - cache[i-2] * 625 + 1;
        }
    }
    return cache[n];
}

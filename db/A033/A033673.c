/*
 * A033673  Trajectory of 79 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 79, a(n) = a(n-1) + reverse_decimal(a(n-1)) for n > 0
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   reverse_decimal, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/reverse_decimal.h"

/* static cache of computed terms */
static seq_t *cache = NULL;   /* array of seq_t */
static idx_t cache_max = 0;   /* largest index computed (valid after first init) */
static size_t cache_cap = 0;  /* capacity of cache array */

/* ensure cache contains terms up to index n */
static void ensure_cache(idx_t n)
{
    if (cache != NULL && n <= cache_max)
        return; /* already ready */

    if (cache == NULL) {
        /* first initialization: allocate space for n=0 term */
        cache_cap = 1;
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)79;
        cache_max = 0;
        if (n == 0)
            return;
        n -= 0; /* continue to compute rest */
    }

    /* grow capacity if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[(size_t)i - 1];
        seq_t rev  = reverse_decimal(prev);
        cache[(size_t)i] = prev + rev;
    }
    cache_max = n;
}

/* sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

/*
 * A041142  Numerators of continued fraction convergents to sqrt(80).
 * https://oeis.org/A041142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041142: Numerators of continued fraction convergents to sqrt(80).
   Recurrence: a(0)=8, a(1)=9, a(2)=152, a(3)=161,
   and for n>=4: a(n)=18*a(n-2)-a(n-4).
   This implementation memoizes results in a static array so
   any order of calls works efficiently. */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of stored terms (max index + 1) */
static size_t cache_cap  = 0;   /* allocated capacity */

static void ensure_cache(idx_t n)
{
    /* Initialize cache if needed. */
    if (cache == NULL) {
        size_t init_cap = 4;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)152;
        cache[3] = (seq_t)161;
        cache_size = 4;
        cache_cap = init_cap;
    }

    /* Grow cache to accommodate index n. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_cap = cache_cap;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        /* Double capacity until enough. */
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms if necessary. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 4) continue;  /* already initialized */
        cache[i] = (seq_t)18 * cache[i - 2] - cache[i - 4];
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

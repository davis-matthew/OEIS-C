/*
 * A129704  Expansion of 1/(x^5 - 2*x^4 + x^3 - 2*x^2 + x - 1).
 * https://oeis.org/A129704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 41, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129704:  Expansion of 1/(x^5 - 2*x^4 + x^3 - 2*x^2 + x - 1)
   Recurrence: a(0) = -1, a(1) = -1, a(2) = 1, a(3) = 2, a(4) = 1;
                a(n) = a(n-1) - 2*a(n-2) + a(n-3) - 2*a(n-4) + a(n-5)  for n ≥ 5 */

#include <stdlib.h>   /* for malloc, realloc, size_t */

static seq_t *cache = NULL;     /* dynamic cache of computed terms   */
static size_t cache_cap = 0;    /* allocated capacity (in seq_t units) */
static size_t cache_size = 0;   /* number of terms currently cached   */

/* Ensure that the cache has at least 'need' slots.   */
static void ensure_cap(size_t need, size_t *cap, seq_t **arr)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? need : (*cap << 1);
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

static const seq_t base[5] = { -1, -1, 1, 2, 1 }; /* a(0) … a(4) */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n < 5) return base[(size_t)n];

    /* Convert idx_t to size_t for indexing (safe for small n) */
    size_t nsize = (size_t)n;

    /* Initialise the cache if it does not contain the base values */
    if (cache == NULL || cache_cap < 5)
    {
        ensure_cap(5, &cache_cap, &cache);
        for (size_t i = 0; i < 5; ++i) cache[i] = base[i];
        cache_size = 5;
    }

    /* If already computed return immediately */
    if (nsize < cache_size) return cache[nsize];

    /* Grow cache to accommodate up to n */
    ensure_cap(nsize + 1, &cache_cap, &cache);

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_size; i <= nsize; ++i)
    {
        /* Offset i-1 … i-5 are guaranteed to exist (since i ≥ 5) */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        cache[i] = a1 - a2 * ((seq_t)2) + a3 - a4 * ((seq_t)2) + a5;
    }

    cache_size = nsize + 1;
    return cache[nsize];
}

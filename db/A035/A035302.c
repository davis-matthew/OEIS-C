/*
 * A035302  a(n+1)=2a(n)-4a(n-1)+4a(n-2).
 * https://oeis.org/A035302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 38, i64 n <= 78, i128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+1) = 2*a(n) - 4*a(n-1) + 4*a(n-2),  a(0)=0, a(1)=1, a(2)=-2 */
#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation (compatible with both C and C++) */
static seq_t *cache      = NULL;   /* array of computed terms                 */
static size_t cache_cap  = 0;      /* allocated capacity (in elements)        */
static size_t cache_len  = 0;      /* number of terms that are initialized   */

/* Ensure that cache contains terms 0 … needed-1 (needed is size_t) */
static void
ensure_cache(size_t needed)
{
    if (needed <= cache_len)          /* already computed */
        return;

    /* enlarge the buffer if necessary – power‑of‑two growth */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL && new_cap != 0)
            abort();                  /* allocation failure – signal error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the initial terms if this is the first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        if (needed > 1)
            cache[1] = (seq_t)1;
        if (needed > 2)
            cache[2] = (seq_t)-2;
        cache_len = (needed >= 3) ? 3 : needed;
    }

    /* fill consecutive terms up to desired size */
    for (size_t i = cache_len; i < needed; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1]
                 - (seq_t)4 * cache[i - 2]
                 + (seq_t)4 * cache[i - 3];
    }
    cache_len = needed;
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    /* Base cases – can be answered without allocating / computing */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-2;

    /* Compute (and cache) up to n */
    ensure_cache((size_t)n + 1);

    return cache[(size_t)n];
}

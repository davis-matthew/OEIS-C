/*
 * A047905  a(n+1) = a(n) + n (if n is even), a(n+1) = a(n) * n (if n is odd).
 * https://oeis.org/A047905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 1; for n >= 1, a(n+1) = a(n) + n if n is even, * a(n+1) =
 *            a(n) * n if n is odd
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 33, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed values, 1‑based */
static idx_t cache_size = 0;         /* largest index for which cache[i] is valid */

/* Ensure that a(n) is computed and cached for all n up to @max_n. */
static void ensure_up_to(idx_t max_n)
{
    if (max_n <= cache_size)
        return;

    /* Allocate or grow the cache */
    if (cache == NULL) {
        /* first allocation: at least one element (index 1) */
        cache = (seq_t *)malloc(sizeof(seq_t) * (max_n + 1));
        if (cache == NULL)
            return;                 /* allocation failure: silently give up */
        cache[1] = (seq_t)1;        /* a(1) = 1 */
        cache_size = 1;
    } else if (max_n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (max_n + 1));
        if (tmp == NULL)
            return;                 /* reallocation failure: silently give up */
        cache = tmp;
        /* new elements will be filled in the loop below */
    }

    /* Compute missing terms from cache_size+1 up to max_n */
    for (idx_t i = cache_size + 1; i <= max_n; ++i) {
        idx_t prev = i - 1;
        seq_t prev_val = cache[prev];
        if ((prev & 1) == 0)          /* prev even: addition */
            cache[i] = prev_val + (seq_t)prev;
        else                          /* prev odd: multiplication */
            cache[i] = prev_val * (seq_t)prev;
    }
    cache_size = max_n;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below offset is irrelevant */
        return (seq_t)0;
    ensure_up_to(n);
    return cache[n];
}

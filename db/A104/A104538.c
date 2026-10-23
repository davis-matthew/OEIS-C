/*
 * A104538  Expansion of g.f. (1 + 2*x) / (1 + 2*x + 4*x^2).
 * https://oeis.org/A104538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   * a(0) = 1 * a(1) = 0 * a(n) = -2*a(n-1) - 4*a(n-2) for n >= 2 *
 *            (generates the expansion of (1+2x)/(1+2x+4x^2))
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 31, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache static storage so that repeated calls are fast */
static seq_t *cache = NULL;          /* array of computed values */
static idx_t cache_size = 0;         /* number of values stored (next index to compute) */
static idx_t cache_cap  = 0;         /* current allocated capacity */

/* ensure the cache has at least n+1 entries allocated */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;          /* already sufficient */

    idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap <= n) new_cap <<= 1;     /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) { /* allocation failed – abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialize for very first call */
    if (cache == NULL) {
        cache = NULL;
        cache_cap = 0;
        cache_size = 0;
    }

    ensure_cache(n);

    /* compute values from current cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)0;
        } else {
            seq_t t1 = (seq_t)2 * cache[i - 1];
            seq_t t2 = (seq_t)4 * cache[i - 2];
            cache[i] = -(t1 + t2);
        }
    }
    cache_size = n + 1;          /* next needed index is n+1 */

    return cache[n];
}

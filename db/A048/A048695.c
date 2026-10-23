/*
 * A048695  Generalized Pellian with second term equal to 8.
 * https://oeis.org/A048695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2); a(0)=1, a(1)=8
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a simple growable cache */
static seq_t *cache = NULL;     /* array of computed terms */
static idx_t cache_cap = 0;     /* number of slots currently allocated */

/* enlarge cache if necessary to hold indices up to target (inclusive) */
static void ensure_capacity(idx_t target)
{
    if (target < cache_cap) /* already enough */
        return;

    idx_t new_cap = target + 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        return;                 /* out of memory – let the caller see garbage */
    /* copy old values */
    for (idx_t i = 0; i < cache_cap; ++i)
        tmp[i] = cache[i];
    free(cache);                /* free old storage */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call and handle small n directly */
    if (cache == NULL) {
        ensure_capacity(1);          /* need indices 0 and 1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        if (n == 0) return cache[0];
        if (n == 1) return cache[1];
    }

    /* compute missing terms up to n */
    idx_t old_size = cache_cap;        /* terms already available */
    if (n >= cache_cap) {
        ensure_capacity(n);            /* enlarge storage */
        for (idx_t i = old_size; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-1) + a(i-2) */
            cache[i] = (seq_t)2 * cache[i - 1] + cache[i - 2];
        }
    }
    return cache[n];
}

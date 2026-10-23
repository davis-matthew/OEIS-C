/*
 * A029234  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 7, 10, and 12.
 *            Computed by the classical coin‑change DP algorithm (parts
 *            processed in outer loop, amount in inner loop)
 * Range:     u8 n <= 119, u16 n <= 851, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const idx_t parts[] = {2, 7, 10, 12};
static const size_t parts_cnt = 4;

/* Return a(n), the number of partitions of n into the set {2,7,10,12}. */
seq_t a(idx_t n)
{
    /* Static cache of computed values and its capacity (n+1). */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Grow cache if necessary and recompute everything up to new n. */
    if (cache_cap <= n) {
        /* Allocate to (n+1) entries. */
        seq_t *tmp = NULL;
        if (cache_cap == 0) {
            /* First call: just allocate. */
            tmp = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        } else {
            /* Realloc to new size. */
            tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        }
        if (tmp == NULL) {
            /* Allocation failed; return 0 as a safe default. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = n + 1;

        /* Initialize all entries to 0. */
        for (idx_t i = 0; i < cache_cap; ++i) {
            cache[i] = (seq_t)0;
        }
        cache[0] = (seq_t)1;             /* a(0) = 1 */

        /* Compute DP table using outer‑parts ordering. */
        for (size_t k = 0; k < parts_cnt; ++k) {
            idx_t p = parts[k];
            for (idx_t i = p; i < cache_cap; ++i) {
                cache[i] += cache[i - p];
            }
        }
    }

    /* cache_cap > n, so the value has already been computed. */
    return cache[n];
}

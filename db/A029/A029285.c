/*
 * A029285  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A029285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 5, 8, and 12
 *            (order of parts does not matter)
 * Range:     u8 n <= 115, u16 n <= 813, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values */
static idx_t cache_size = 0;  /* number of computed terms (cache length) */

/* Recompute the whole sequence up to new_limit (inclusive).  Uses the
   standard coin‑change DP that counts unordered partitions. */
static void recompute_cache(idx_t new_limit)
{
    size_t sz = (size_t)(new_limit + 1);
    seq_t *tmp = (seq_t *)realloc(cache, sz * sizeof(seq_t));
    if (tmp == NULL) return;          /* allocation failure – caller will deal */
    cache = tmp;

    /* reset to zero, then set base case a(0)=1 */
    for (idx_t i = 1; i <= new_limit; ++i) cache[i] = 0;
    cache[0] = (seq_t)1;

    /* coin denominations */
    const idx_t coins[4] = {3, 5, 8, 12};

    /* standard DP: for each coin, add contributions to all higher sums */
    for (int ci = 0; ci < 4; ++ci) {
        idx_t c = coins[ci];
        if (c > new_limit) continue;
        for (idx_t i = c; i <= new_limit; ++i) {
            cache[i] += cache[i - c];
        }
    }

    cache_size = new_limit + 1;
}

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        recompute_cache(n);
    }
    return cache[n];
}

/*
 * A131508  2*A000027 (natural numbers) sandwiched by tripled A001477
 *          (nonnegative numbers).
 * https://oeis.org/A131508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*a(n-4) - a(n-8) for n > 7 a(0)=0, a(1)=0, a(2)=0,
 *            a(3)=2, a(4)=1, a(5)=1, a(6)=1, a(7)=4 (initial values are
 *            hard‑coded; recurrence builds later terms)
 * Range:     u8 n <= 510, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence cache */
static seq_t *cache = NULL;     /* array holding computed terms   */
static idx_t  cache_cap = 0;    /* allocated array capacity       */
static idx_t  max_comp   = 0;   /* highest index that has been computed   */

/* hard‑coded initial 8 values */
static const seq_t init[8] = {0, 0, 0, 2, 1, 1, 1, 4};

/* Ensure the cache has room for index n (inclusive).  May grow the array. */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Return immediately for the base cases */
    if (n < 8)
        return init[n];

    /* Make sure we have storage for all indices up to n */
    ensure_cache(n);

    /* If we have not yet computed the first 8 terms, store them */
    if (max_comp < 7) {
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        max_comp = 7;
    }

    /* Compute any terms that are still missing (from max_comp+1 up to n) */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-4) - a(i-8) */
        cache[i] = cache[i-4];
        cache[i] += cache[i-4];    /* double it */
        cache[i] -= cache[i-8];
    }
    max_comp = n;

    return cache[n];
}

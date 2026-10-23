/*
 * A009283  E.g.f.: exp(x + sinh(x)).
 * https://oeis.org/A009283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0) = 1 a(n) = a(n-1) + Σ_{k=0..⌊(n-1)/2⌋} binomial(n-1,
 *            2k) * a(n-2k-1) for n ≥ 1 (OEIS A009283: E.g.f.: exp(x +
 *            sinh(x)) )
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Cached values of the sequence */
static seq_t *cache = NULL;          /* array of computed terms          */
static size_t  cap   = 0;            /* current capacity of cache        */
static idx_t   computed = 0;         /* number of terms that are defined */

/* Ensure the cache has room for indices up to n, allocating as needed */
static void ensure_cache_size(idx_t n)
{
    if (n < cap)
        return;
    size_t new_cap = cap ? cap << 1 : 1;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure – abort; the test harness assumes success on a
           small capacity. */
        abort();
    }
    cache = tmp;
    /* Initialise new entries to zero (not strictly necessary for seq_t). */
    for (size_t i = cap; i < new_cap; ++i)
        cache[i] = 0;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise base value a(0) = 1 if not already done */
    if (computed == 0) {
        ensure_cache_size(0);
        cache[0] = 1;
        computed = 1;
    }

    if (n == 0)
        return cache[0];

    /* Grow cache to accommodate n */
    ensure_cache_size(n);

    /* Compute terms from the first uncomputed index up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* i ≥ 1 here */
        seq_t sum = 0;
        idx_t maxk = (i - 1) / 2;
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t two_k = 2 * k;
            seq_t c = binomial(i - 1, two_k);
            seq_t term = c * cache[i - two_k - 1];
            sum += term;
        }
        cache[i] = cache[i - 1] + sum;
    }
    computed = n + 1;
    return cache[n];
}

/*
 * A168999  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. * For n ≥ 1: * a(n) = 32·∑_{k=1}^{min(22,n)} a(n–k) –
 *            528·a(n–23) + e(n), * where e(n) = 2 for 1 ≤ n ≤ 22, e(23) = 1,
 *            and e(n) = 0 otherwise. * This recurrence is equivalent to the
 *            rational generating function * (t^23 + 2t^22 + … + 2t + 1) / (1
 *            – 32t – … – 32t^22 + 528t^23). * * The fun...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *cache      = NULL;   /* cache of computed terms  */
static size_t cache_len  = 0;      /* number of stored terms   */
static size_t cache_cap  = 0;      /* allocated capacity       */

/* Ensure the cache can hold at least indices 0..n inclusive. */
static void
ensure_cache_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = tmp;
    cache_cap = new_cap;
}

/* Compute all terms up to index n (inclusive). */
static void
compute_up_to(idx_t n)
{
    if (cache == NULL) {
        /* Allocate initial space for the first term. */
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 1;
        cache[0]  = (seq_t)1;          /* a(0) */
    }

    if (n < cache_len) /* already computed */
        return;

    ensure_cache_capacity(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum22 = 0;
        for (idx_t k = 1; k <= 22 && i >= k; ++k)
            sum22 += cache[i - k];

        seq_t term = (seq_t)32 * sum22;
        if (i >= 23)
            term -= (seq_t)528 * cache[i - 23];

        if (i >= 1 && i <= 22)
            term += (seq_t)2;
        else if (i == 23)
            term += (seq_t)1;

        cache[i] = term;
    }

    cache_len = (size_t)n + 1;
}

seq_t
a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}

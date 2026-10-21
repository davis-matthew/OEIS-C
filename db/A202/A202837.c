/*
 * A202837  E.g.f.: exp(3*x + 2*x^2).
 * https://oeis.org/A202837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A202837
   a(0) = 1, a(1) = 3
   Recurrence: a(n) = 3*a(n-1) + 4*(n-1)*a(n-2)  (n ≥ 2) */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values a(0)..a(cache_cap-1) */
static idx_t  cache_cap = 0;         /* current cache capacity */
static idx_t  computed   = 0;        /* number of values already computed */

/* Ensure the cache can hold values up to n inclusive. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();            /* allocation failure */

    cache = new_cache;
    cache_cap = new_cap;

    /* Initialise a(0) and a(1) once. */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        if (n >= 1)
            cache[1] = (seq_t)3;
        computed = (n >= 1 ? 2 : 1);
    }
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    ensure_cache(n);

    /* Compute values from the next uncomputed index up to n. */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t term1 = (seq_t)3 * cache[i - 1];
        seq_t term2 = ((seq_t)4 * (seq_t)(i - 1)) * cache[i - 2];
        cache[i] = term1 + term2;
    }
    computed = n + 1;
    return cache[n];
}

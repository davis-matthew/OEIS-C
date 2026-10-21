/*
 * A097551  Number of positive words of length n in the monoid Br_4 of
 *          positive braids on 5 strands.
 * https://oeis.org/A097551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-3) - 3*a(n-4) + a(n-5)
 * with initial values a(0)=1, a(1)=4, a(2)=13, a(3)=37, a(4)=101
 */
#include <stdlib.h>

/* static cache for previously computed values */
static seq_t *cache = NULL;        /* array of computed a(n) values          */
static size_t cache_cap = 0;       /* current allocated capacity             */
static idx_t last_computed = (idx_t)-1; /* highest index with a valid value */

/* Ensure that cache has space for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();                     /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* Lazy initialization of the first five terms */
    if (last_computed == (idx_t)-1) {
        ensure_cache(4);              /* guarantee capacity ≥ 5 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)37;
        cache[4] = (seq_t)101;
        last_computed = 4;
    }

    /* If requested value is already computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* Grow cache if necessary and compute missing terms */
    ensure_cache(n);
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term = (seq_t)4 * cache[i-1];
        term -= (seq_t)5 * cache[i-2];
        term += (seq_t)5 * cache[i-3];
        term -= (seq_t)3 * cache[i-4];
        term +=       cache[i-5];
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}

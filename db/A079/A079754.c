/*
 * A079754  Operation count to create all permutations of n distinct elements
 *          using the "streamlined" version of Algorithm L (lexicographic
 *          permutation generation) from Knuth's The Art of Computer
 *          Programming, Vol. 4, chapter 7.2.1.2. Sequence gives number of
 *          times l has to be repeatedly decreased in step L3...
 * https://oeis.org/A079754
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-2)*(n-3)/2 with a(3) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation data */
static seq_t *cache   = NULL;
static idx_t cache_cap = 0;
static idx_t last_computed = 0;

/* Ensure cache can hold indices up to n */
static void ensure_cache(idx_t n) {
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap == 0 ? 4 : cache_cap;
    while (new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;  /* memory failure – return 0 below */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function */
seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;        /* behaviour below offset is ignored */

    ensure_cache(n);

    /* Initialise cache[3] once */
    if (!last_computed) {
        cache[3] = (seq_t)0;
        last_computed = 3;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)i * cache[i - 1];
        seq_t term2 = ((seq_t)(i - 2) * (seq_t)(i - 3)) / 2;
        cache[i] = term1 + term2;
    }
    last_computed = n > last_computed ? n : last_computed;

    return cache[n];
}

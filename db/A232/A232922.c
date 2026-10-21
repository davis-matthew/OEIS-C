/*
 * A232922  Number of 3 X n 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) – 2*a(n-3) for n>4 Initial values
 *            (offset 1): a(1)=27, a(2)=54, a(3)=246, a(4)=912
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static state: cache of computed terms */
static seq_t *cache = NULL;    /* array indices 0..cache_cap   */
static idx_t  cache_cap = 0;   /* highest allocated index     */
static idx_t  last_computed = 0; /* highest index that has a value */

/* ensure the cache can store terms up to `need`.  The array will hold
   indices 0..need; index 0 is unused. */
static void
ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;

    idx_t new_cap = need;           /* highest index to store */
    idx_t new_size = new_cap + 1;   /* number of elements                  */
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));

    if (!new_cache)                 /* allocation failed – give up */
        return;

    /* copy already computed values */
    if (cache != NULL && last_computed <= cache_cap) {
        for (idx_t i = 0; i <= last_computed; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of OEIS A232922.  n>=1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* initialise cache and first four terms on first call */
    if (last_computed == 0) {
        ensure_capacity(4);                 /* indices 0..4  */
        cache[0] = (seq_t)0;                /* unused slot  */
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)54;
        cache[3] = (seq_t)246;
        cache[4] = (seq_t)912;
        last_computed = 4;
    }

    if (n <= last_computed)
        return cache[n];

    /* extend cache and compute new terms */
    ensure_capacity(n);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* use 128‑bit arithmetic to avoid intermediate overflow
           when seq_t is 64‑bit.  The cast back to seq_t is safe
           because the final result always fits in seq_t. */
        seq_t term = (seq_t)(((__int128)3 * cache[i-1]) +
                             ((__int128)4 * cache[i-2]) -
                             ((__int128)2 * cache[i-3]));
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}

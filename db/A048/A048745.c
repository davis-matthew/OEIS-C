/*
 * A048745  Partial sums of A048654.
 * https://oeis.org/A048745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 3 a(0) = 1, a(1) = 5
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

/* Local cache of computed terms. */
static seq_t *cache = NULL;   /* array of cache values       */
static size_t cap   = 0;       /* capacity of cache array     */
static size_t have  = 0;       /* number of terms already computed (cache[0..have-1]) */

/* Grow the cache to hold at least 'needed' elements. */
static void grow_cache(size_t needed) {
    if (needed <= cap) return;
    size_t new_cap = cap ? cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n) {
    /* If the value is already computed, return it. */
    if (n < have) return cache[n];

    /* Make sure the cache can hold all values up to n. */
    grow_cache((size_t)(n + 1));

    /* Compute any missing terms. */
    for (idx_t i = (idx_t)have; i <= n; ++i) {
        if (i == 0)            cache[i] = (seq_t)1;
        else if (i == 1)       cache[i] = (seq_t)5;
        else                   cache[i] = 2 * cache[i-1] + cache[i-2] + (seq_t)3;
    }
    have = (size_t)(n + 1);
    return cache[n];
}

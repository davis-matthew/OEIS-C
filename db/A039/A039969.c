/*
 * A039969  An example of a d-perfect sequence: a(n) = Catalan(n) mod 3.
 * https://oeis.org/A039969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) mod 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of Catalan numbers modulo 3 */
static seq_t *cache = (seq_t *)0;
static idx_t cache_len = 0;

/* Ensure that cache contains values up to index n (inclusive) */
static void
ensure_cache(idx_t n)
{
    idx_t i, j;
    seq_t sum, prod;

    if (cache_len > n)
        return;

    /* Allocate or grow the cache. */
    if (cache_len == 0) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (cache == (seq_t *)0) {
            /* Allocation failure: abort. */
            abort();
        }
        cache[0] = (seq_t)1;   /* Catalan(0) = 1 */
        cache_len = 1;
    } else {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (cache == (seq_t *)0) {
            abort();
        }
    }

    /* Compute new entries. */
    for (i = cache_len; i <= n; ++i) {
        sum = (seq_t)0;
        for (j = 0; j <= i - 1; ++j) {
            prod = cache[j] * cache[i - 1 - j];
            prod %= (seq_t)3;
            sum += prod;
            sum %= (seq_t)3;   /* keep the sum within 0..2 */
        }
        cache[i] = sum;
    }
    cache_len = n + 1;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

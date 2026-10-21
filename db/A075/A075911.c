/*
 * A075911  Third column of triangle A075500.
 * https://oeis.org/A075911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - 275*a(n-2) + 750*a(n-3) (a(0)=1, a(1)=30,
 *            a(2)=625)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size = 0;          /* number of cached elements */

    /* Initialize cache with first three terms on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)625;
        size = 3;
    }

    /* Return cached value if already computed */
    if (n < size) return cache[n];

    /* Grow cache to hold up to index n */
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        seq_t t1 = (seq_t)30 * cache[i - 1];
        seq_t t2 = (seq_t)275 * cache[i - 2];
        seq_t t3 = (seq_t)750 * cache[i - 3];
        cache[i] = t1 - t2 + t3;
    }

    size = n + 1;
    return cache[n];
}

/*
 * A144297  BINOMIAL transform of A001515.
 * https://oeis.org/A144297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*a(n-1) - (4*n-5)*a(n-2) + 2*(n-2)*a(n-3) with
 *            a(0) = 1, a(1) = 3, a(2) = 12
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"  /* ensures alloc_seq is available */
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* Handle the defined initial values. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)12;

    /* Static cache of computed values. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of allocated elements */
    static idx_t last_done = 2;     /* last index for which we have a value */

    /* Initialize the cache on first call. */
    if (!cache) {
        cache = alloc_seq(3);          /* allocate space for indices 0..2 */
        cache_size = 3;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)12;
    }

    /* If we already have the requested value, return it. */
    if (n <= last_done)
        return cache[n];

    /* Ensure that the cache can hold index n. */
    ensure_cache(n, &cache, &cache_size);

    /* Compute all values up to n and store them. */
    for (idx_t i = last_done + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)(2 * i + 1) * cache[i - 1];
        seq_t term2 = (seq_t)(4 * i - 5) * cache[i - 2];
        seq_t term3 = (seq_t)2 * (i - 2) * cache[i - 3];
        cache[i] = term1 - term2 + term3;   /* safe: intermediate fits when result fits */
    }

    last_done = n;
    return cache[n];
}

/*
 * A232163  Cardinality of the Weyl alternation set corresponding to the
 *          zero-weight in the adjoint representation of the Lie algebra
 *          so(2n+1).
 * https://oeis.org/A232163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+3*a(n-3)+a(n-4);
 *            a(0)=0,a(1)=1,a(2)=2,a(3)=5
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* Array containing computed terms */
    static idx_t cache_size = 0;  /* Number of elements currently stored */

    /* Remember previous size before any possible re‑allocation */
    idx_t old_size = cache_size;

    /* First call – allocate and initialise the first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        old_size = cache_size;
    }

    /* If the requested index is beyond the current capacity, grow the array */
    if (cache_size <= n) {
        size_t new_capacity = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_capacity);
        cache_size = (idx_t)new_capacity;
    }

    /* Compute any missing terms using the recurrence relation */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t v = cache[i-1] + cache[i-2] + 3 * cache[i-3] + cache[i-4];
        cache[i] = v;
    }

    return cache[n];
}

/*
 * A163839  a(n) = (2^n-1)*4^(2*n-1).
 * https://oeis.org/A163839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1) * 4^(2*n - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 4, a(n) = 48*a(n-1) - 512*a(n-2) for n > 1 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static idx_t cache_len = 0;   /* number of allocated entries */
    static idx_t next = 0;        /* next index to compute */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure, safe fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        next = 2;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate index n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0; /* allocation failure, safe fallback */
    cache = tmp;
    cache_len = new_len;

    /* compute missing terms using the recurrence */
    for (idx_t k = next; k <= n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)0;          /* safety, already set */
        } else if (k == 1) {
            cache[1] = (seq_t)4;          /* safety, already set */
        } else {
            cache[k] = 48 * cache[k - 1] - 512 * cache[k - 2];
        }
    }
    next = n + 1;

    return cache[n];
}

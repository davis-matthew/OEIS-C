/*
 * A097833  Partial sums of Chebyshev sequence S(n,20)= U(n,10) =
 *          A075843(n+1).
 * https://oeis.org/A097833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - a(n-2) + 1 (a(0)=1, a(1)=21)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A097833 (partial sums of Chebyshev U(n,10)) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow non‑monotonic calls */
    static seq_t *cache = NULL;
    static idx_t cached_cnt = 0; /* number of terms currently stored (max index + 1) */

    /* Initialize cache with a(0) = 1 */
    if (cached_cnt == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cache[0] = (seq_t)1;
        cached_cnt = 1;
    }

    /* If the requested index is already cached, return it */
    if (n < cached_cnt) {
        return cache[n];
    }

    /* Need to extend cache up to n */
    /* Resize array */
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute missing terms */
    for (idx_t i = cached_cnt; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)21;                     /* a(1) */
        } else {                                 /* i >= 2 */
            val = (seq_t)20 * cache[i-1] - cache[i-2] + (seq_t)1;
        }
        cache[i] = val;
    }

    cached_cnt = n + 1;
    return cache[n];
}

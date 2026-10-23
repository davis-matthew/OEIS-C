/*
 * A108775  a(n) = floor(sigma(n)/n).
 * https://oeis.org/A108775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sigma(n)/n)
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) = ⌊σ(n)/n⌋ for n ≥ 1.
   σ(n) is the sum of all positive divisors of n.
   The function caches intermediate results so that multiple
   calls with arbitrary n are efficient. */
seq_t a(idx_t n)
{
    /* cache[0] is unused; cache[i] holds a(i) for i ≥ 1 */
    static seq_t *cache = NULL;   /* cached a(i) values, 0..cache_size-1 */
    static idx_t cache_size = 0;  /* number of elements currently stored */

    if (n == 0) return 0;   /* safety guard – sequence is defined for n ≥ 1 */

    if (n >= cache_size) {
        /* need to extend cache up to n (inclusive) */
        size_t new_size = (size_t)n + 1;                /* elements 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                            /* allocation failure */
        cache = tmp;

        /* compute a(i) for any not-yet-computed indices */
        idx_t start = (cache_size > 1 ? cache_size : 1); /* avoid i = 0 */
        for (idx_t i = start; i <= n; ++i) {
            seq_t sigma = 0;
            /* sum of divisors of i */
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    sigma += (seq_t)d;
                    idx_t other = i / d;
                    if (other != d) sigma += (seq_t)other;
                }
            }
            cache[i] = sigma / (seq_t)i;   /* integer division = floor */
        }
        cache_size = new_size;
    }

    return cache[n];
}

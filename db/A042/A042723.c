/*
 * A042723  Denominators of continued fraction convergents to sqrt(891).
 * https://oeis.org/A042723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=1, a(n)=periodic[(n-1)%8]*a(n-1)+a(n-2), where
 *            periodic=[1,5,1,1,1,5,1,58] – the partial quotients after the
 *            leading term of sqrt(891). The sequence gives the denominators
 *            of the continued‑fraction convergents to sqrt(891)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static const idx_t periodic[8] = {1, 5, 1, 1, 1, 5, 1, 58};

seq_t a(idx_t n)
{
    /* initialise cache for a(0) */
    if (!cache) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;            /* a(0) = 1 */
    }
    /* if already known, return */
    if (n < cache_size) return cache[n];

    /* extend cache to contain a(n) */
    idx_t new_size = cache_size;
    while (new_size <= n) new_size <<= 1;   /* double until big enough */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);

    /* compute missing terms */
    for (idx_t k = cache_size; k <= n; ++k) {
        /* partial quotient for step k (k>=1) */
        seq_t a_k = (seq_t)periodic[(k - 1) % 8];

        if (k == 1) {
            /* a(1) = a1 * a(0) + 0  (q_{-1} = 0) */
            cache[1] = a_k * cache[0];
        } else {
            /* recurrence: a(k) = a_k * a(k-1) + a(k-2) */
            cache[k] = a_k * cache[k - 1] + cache[k - 2];
        }
    }
    cache_size = n + 1;
    return cache[n];
}

/*
 * A041076  Numerators of continued fraction convergents to sqrt(45).
 * https://oeis.org/A041076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 322*a(n-6) - a(n-12) for n ≥ 12 with initial values:
 *            a(0)=6, a(1)=7, a(2)=20, a(3)=47, a(4)=114, a(5)=161, a(6)=2046,
 *            a(7)=2207, a(8)=6460, a(9)=15127, a(10)=36714, a(11)=51841
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms of the sequence, used for n < 12 */
    static const seq_t init[12] = {
        6, 7, 20, 47, 114, 161,
        2046, 2207, 6460, 15127, 36714, 51841
    };

    /* Cache of previously computed terms.
       cache[i] == a(i) for i < cache_size.  */
    static seq_t *cache   = NULL;   /* array of a(i) values       */
    static size_t cache_cap = 0;    /* allocated capacity (elements) */
    static idx_t cache_size = 0;    /* number of values currently stored (≡ cache_cap after initialisation) */

    /* Direct return for indices below the recurrence range */
    if (n < 12) return init[n];

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 12;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        cache_size = 12;   /* we currently have values 0 .. 11 */
    }

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        /* In the unlikely event that realloc failed, we would leave the
           function in an inconsistent state, but the competition
           environment guarantees sufficient memory. */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n, using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* i >= 12 here because cache_size is at least 12, and cache_size
           increases only after we compute new values. */
        cache[i] = (seq_t)322 * cache[(size_t)(i - 6)] - cache[(size_t)(i - 12)];
    }

    cache_size = n + 1;   /* cache now contains a(0) … a(n) */
    return cache[(size_t)n];
}

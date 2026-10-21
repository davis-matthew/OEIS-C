/*
 * A233522  Expansion of 1 / (1 - x - x^4 + x^9) in powers of x.
 * https://oeis.org/A233522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-9); a(0)=1, a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=2
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 79, u64 n <= 158,
 *            u128 n <= 316, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache will hold the sequence values up to the largest
       index that has been computed so far. */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    /* Initialise cache with the first five terms. */
    if (cache == NULL) {
        size_t init_cap = 5;                     /* indices 0..4 */
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failure: safe fallback */

        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 2;
        max_computed = 4;
    }

    /* Return if the requested value is already cached. */
    if (n <= max_computed) return cache[n];

    /* Grow the cache if necessary. */
    size_t needed = (size_t)n + 1;                /* n is unsigned */
    if (needed > (size_t)(max_computed + 1)) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* allocation failure: safe fallback */
        cache = tmp;
    }

    /* Compute all terms from max_computed+1 up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = (i >= 4) ? cache[i - 4] : 0;
        seq_t v3 = (i >= 9) ? cache[i - 9] : 0;
        cache[i] = v1 + v2 - v3;
    }

    max_computed = n;
    return cache[n];
}

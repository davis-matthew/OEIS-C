/*
 * A203230  (n-1)-st elementary symmetric function of the first n terms of
 *          A010684.
 * https://oeis.org/A203230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) - 9*a(n-4), a(1)=1, a(2)=4, a(3)=7, a(4)=24
 *            (offset 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Zero index is unused */
    if (n < 1) return 0;

    static seq_t *cache     = NULL;   /* memoization array */
    static idx_t cached_n   = 0;      /* highest n computed so far */
    static idx_t capacity   = 0;      /* allocated size (indices 0..capacity-1) */

    /* First call: allocate and fill initial values */
    if (cache == NULL) {
        capacity = (n > 4 ? n + 1 : 5);      /* ensure space up to max(4,n) */
        cache = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (cache == NULL) return 0;          /* allocation failure */

        cache[0] = 0;                         /* unused */
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 7;
        cache[4] = 24;

        cached_n = (capacity > 4 ? 4 : capacity - 1);
    }

    /* If cache needs to grow for this n */
    if (n >= capacity) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (new_cache == NULL) {
            free(cache);
            cache = NULL;
            cached_n = 0;
            capacity = 0;
            return 0;                          /* allocation failure */
        }
        cache = new_cache;
        capacity = new_cap;
    }

    /* Return already computed value */
    if (n <= cached_n) return cache[n];

    /* Compute values from cached_n+1 up to n using the recurrence */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        if (i <= 4) {
            /* Base cases already stored */
            continue;
        }
        cache[i] = (seq_t)6 * cache[i - 2] - (seq_t)9 * cache[i - 4];
    }
    cached_n = n;
    return cache[n];
}

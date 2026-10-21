/*
 * A057198  a(n) = (5*3^(n-1)+1)/2.
 * https://oeis.org/A057198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*3^(n-1)+1)/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(n)=3*a(n-1)-1 for n>1 */

#include <stdlib.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (undefined by sequence), but avoid UB */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;   /* cache[0] holds a(1) */
    static idx_t cached   = 0;    /* number of terms cached (max n computed) */

    /* If needed, extend the cache */
    if (cached < n) {
        size_t new_size = (size_t)n;               /* number of entries needed */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – return 0 as safe fallback */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialize the first term if we haven't yet */
        if (cached == 0) {
            cache[0] = (seq_t)3; /* a(1) = 3 */
            cached   = 1;
        }

        /* Compute subsequent terms using the recurrence */
        for (idx_t i = cached; i < n; ++i) {
            /* cache[i] corresponds to a(i+1) */
            cache[(size_t)i] = cache[(size_t)(i - 1)] * (seq_t)3 - (seq_t)1;
        }

        cached = n;
    }

    /* Return the cached value for a(n) (offset 1) */
    return cache[(size_t)(n - 1)];
}

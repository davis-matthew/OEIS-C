/*
 * A206565  Expansion of 1/(1 - 37*x + x^2).
 * https://oeis.org/A206565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 37; a(n) = 37*a(n-1) - a(n-2) for n ≥ 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow out-of-order calls */
    static seq_t *cache = NULL;   /* array of already-computed terms */
    static idx_t max_computed = 0;  /* highest index that has been computed */

    /* Ensure cache is large enough to hold at least a(1) */
    if (cache == NULL) {
        /* Allocate space for indices 0 and 1 */
        size_t initial_size = 2 * sizeof(seq_t);
        cache = (seq_t *)malloc(initial_size);
        if (cache == NULL) abort(); /* memory allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)37;
        max_computed = 1;
    }

    /* Compute additional terms up to n if necessary */
    if (n > max_computed) {
        /* Allocate new larger array */
        size_t new_size_bytes = (size_t)(n + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, new_size_bytes);
        if (tmp == NULL) abort(); /* allocation failure */
        cache = tmp;

        /* Fill new entries using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* 37*a(i-1) - a(i-2) */
            cache[i] = ((seq_t)37 * cache[i - 1]) - cache[i - 2];
        }
        max_computed = n;
    }

    return cache[n];
}

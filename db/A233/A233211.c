/*
 * A233211  Number of n X 2 0..5 arrays with no element x(i,j) adjacent to
 *          value 5-x(i,j) horizontally or antidiagonally, top left element
 *          zero, and 1 appearing before 2 3 and 4, and 2 appearing before 3
 *          in row major order.
 * https://oeis.org/A233211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (75 + 10*9^n + 3*25^n) / 120. Recurrence (valid for n ≥
 *            4): a(n) = 35*a(n-1) - 259*a(n-2) + 225*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a static cache so that repeated, out‑of‑order
   calls are answered in O(1) after the required terms have been
   computed.  All arithmetic is performed in seq_t, which is
   guaranteed to support the required integer operations. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* At least one term is defined (offset 1).  The implementation
       works for all n ≥ 1.  For n < 1 the behaviour is undefined,
       but those calls will not be made in the tests. */
    if (n == 0) {
        /* Undefined for n = 0, but return 0 to avoid UB. */
        return (seq_t)0;
    }

    /* Static cache and its current maximum pre‑computed index. */
    static seq_t *cache = NULL;   /* indices 0..max held */
    static idx_t  cache_max = 0;   /* largest index with a value */

    /* Initialise cache lazily. */
    if (cache == NULL) {
        /* Allocate space for indices 0..3 (index 0 unused). */
        cache = (seq_t *)calloc(4, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        /* Set first three terms (offset 1). */
        cache[1] = (seq_t)2;          /* n = 1  → 2  */
        cache[2] = (seq_t)23;         /* n = 2  → 23 */
        cache[3] = (seq_t)452;        /* n = 3  → 452 */

        cache_max = 3;
    }

    /* Extend cache if the requested index lies beyond the current
       maximum.  Grow the buffer in one step so that future calls
       are fast. */
    if (n > cache_max) {
        idx_t new_max  = n;
        size_t new_cap = (size_t)(new_max + 1);   /* include index 0 */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;        /* allocation failure */
        cache = tmp;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t term = (seq_t)35 * cache[i - 1];
            term += (seq_t)225 * cache[i - 3];
            term -= (seq_t)259 * cache[i - 2];
            cache[i] = term;
        }
        cache_max = n;
    }

    /* All terms are now in cache; return the requested one. */
    return cache[n];
}

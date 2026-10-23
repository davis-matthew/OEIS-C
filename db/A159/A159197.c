/*
 * A159197  Numerator of Hermite(n, 2/9).
 * https://oeis.org/A159197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n+1) = 4*a(n) - 162*n*a(n-1), a(0)=1, a(1)=4
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache to store values already computed -------------------------------- */
    static seq_t *cache = NULL;
    static idx_t  max_computed = 0;
    static int    initialized  = 0;

    /* First call: allocate minimal storage and set a(0) and a(1) if requested */
    if (!initialized) {
        initialized = 1;
        size_t init_size = (n >= 1) ? 2 : 1;           /* at least index 0 */

        cache = (seq_t*)malloc(sizeof(seq_t) * init_size);
        cache[0] = 1;                                 /* a(0) */
        if (n >= 1) {
            cache[1] = 4;                             /* a(1) */
            max_computed = 1;
        } else {
            max_computed = 0;
        }
    }

    /* If value already computed, just return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Grow cache up to the requested index n --------------------------------- */
    size_t new_size = (size_t)(n + 1);
    cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));

    /* Compute missing terms using the recurrence ------------------------------ */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)4 * cache[i - 1];
        seq_t term2 = (seq_t)162 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term1 - term2;
    }

    max_computed = n;
    return cache[n];
}

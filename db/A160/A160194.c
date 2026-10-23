/*
 * A160194  Numerator of Hermite(n, 9/28).
 * https://oeis.org/A160194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 9*a(n-1) - 392*(n-1)*a(n-2), a(0)=1, a(1)=9 (where a(n) =
 *            14^n * Hermite(n, 9/28)) The recurrence is derived from the
 *            physicists’ Hermite recurrence H_n(x) = 2x H_{n-1}(x) - 2(n-1)
 *            H_{n-2}(x) evaluated at x = 9/28
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* computed values */
static idx_t max_n = 0;          /* highest indexed value stored */
static int  initialized = 0;     /* bool */

seq_t a(idx_t n)
{
    /* Initialisation – allocate storage for at least max(n,1) + 1 entries */
    if (!initialized) {
        idx_t init_size = (n > 1) ? n + 1 : 2;          /* at least two slots */
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!cache) return 0;                          /* allocation failed */
        cache[0] = 1;
        cache[1] = 9;
        max_n = init_size - 1;
        initialized = 1;
    }

    /* Extend cache if necessary */
    if (n > max_n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                   /* reallocation failed */
        cache = tmp;
        /* Compute missing terms using the recurrence */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t term1 = 9 * cache[i - 1];
            seq_t factor = (seq_t)392 * (seq_t)(i - 1);
            seq_t term2 = factor * cache[i - 2];
            cache[i] = term1 - term2;
        }
        max_n = n;
    }

    return cache[n];
}

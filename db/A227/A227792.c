/*
 * A227792  Expansion of (1 + 6*x + 17*x^2 - x^3 - 3*x^4)/(1 - 6*x^2 + x^4).
 * https://oeis.org/A227792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=23, a(3)=35, a(4)=134 for n≥5: a(n) =
 *            6*a(n-2) – a(n-4)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of terms currently stored (0 … len-1) */

    /* Grow cache if we need to address a new index */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? 8 : cap * 2;
        if (new_cap < (size_t)n + 5)
            new_cap = (size_t)n + 5;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* memory allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)6;
        else if (i == 2)
            cache[i] = (seq_t)23;
        else if (i == 3)
            cache[i] = (seq_t)35;
        else if (i == 4)
            cache[i] = (seq_t)134;
        else
            cache[i] = (seq_t)6 * cache[i - 2] - cache[i - 4];
    }
    len = (size_t)n + 1;   /* update number of cached terms */

    return cache[n];
}

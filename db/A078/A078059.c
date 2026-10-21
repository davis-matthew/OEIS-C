/*
 * A078059  Expansion of (1-x)/(1+2*x-x^2+2*x^3).
 * https://oeis.org/A078059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -3, a(2) = 7, a(n) = -2*a(n-1) + a(n-2) -
 *            2*a(n-3) for n > 2
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 21, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache    = NULL;  /* array of seq_t values          */
    static idx_t  cap      = 0;     /* allocated capacity               */
    static idx_t  used     = 0;     /* highest index + 1 that is valid */

    /* initialise base values once */
    if (cap == 0) {
        cap = 3;                    /* at least 0..2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* on unlikely OOM return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)(-3);
        cache[2] = (seq_t)7;
        used = 3;
    }

    /* if requested value has been computed, return it */
    if (n < used)
        return cache[n];

    /* enlarge capacity if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* on OOM return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = used; i <= n; ++i) {
        cache[i] = ((seq_t)-2) * cache[i-1]
                 +      cache[i-2]
                 + ((seq_t)-2) * cache[i-3];
    }
    used = n + 1;  /* all indices 0..n now valid */

    return cache[n];
}

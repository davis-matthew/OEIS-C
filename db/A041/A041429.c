/*
 * A041429  Denominators of continued fraction convergents to sqrt(230).
 * https://oeis.org/A041429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 182*a(n-2) - a(n-4) initial values: a(0)=1, a(1)=6,
 *            a(2)=181, a(3)=1092
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n < 4) {
        switch ((size_t)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)6;
            case 2: return (seq_t)181;
            case 3: return (seq_t)1092;
        }
    }

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t max_computed = 0; /* highest index computed so far */

    size_t i, need = (size_t)n + 1;

    /* expand cache if necessary */
    if (cache_len < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_len = need;
    }

    /* initialise the first four terms once */
    if (max_computed < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)181;
        cache[3] = (seq_t)1092;
        max_computed = 3;
    }

    /* compute any missing terms up to n */
    for (i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)182) * cache[i - 2] - cache[i - 4];
    }
    max_computed = (size_t)n;

    return cache[(size_t)n];
}

/*
 * A116793  Number of permutations of length n which avoid the patterns 1432,
 *          2143, 3124; or avoid the patterns 1432, 2314, 3142.
 * https://oeis.org/A116793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) -12*a(n-2) + 11*a(n-3) - 3*a(n-4) + a(n-5) a(1)
 *            = 1, a(2) = 2, a(3) = 6, a(4) = 21, a(5) = 74 (derived from the
 *            rational generating function of A116793)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t sz = 0;          /* last computed index */
    static size_t cap = 0;        /* allocated capacity (>= sz) */

    /* initialise cache with first 5 terms */
    if (cache == NULL) {
        /* at least need space for indices 0..5 */
        cap = 6;
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure, unlikely */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)74;
        sz = 5;
    }

    /* increase capacity if required */
    if ((idx_t)cap < n) {
        while ((idx_t)cap < n) cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure, unlikely */
        cache = tmp;
    }

    /* return cached value if already computed */
    if (n <= sz) return cache[n];

    /* compute new values up to n using the recurrence */
    for (idx_t k = sz + 1; k <= n; ++k) {
        seq_t term = ((seq_t)6) * cache[k-1];
        term -= ((seq_t)12) * cache[k-2];
        term += ((seq_t)11) * cache[k-3];
        term -= ((seq_t)3) * cache[k-4];
        term += cache[k-5];
        cache[k] = term;
    }
    sz = n;
    return cache[n];
}

/*
 * A140710  Number of maximal initial consecutive columns ending at the same
 *          level, summed over all deco polyominoes of height n.
 * https://oeis.org/A140710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) - 3*(n-2)*a(n-2) +
 *            (n+3)*a(n-1) with recurrence a(n) = (n+3)*a(n-1) - 3*n*a(n-2) +
 *            2*(n-2)*a(n-3) and initial values a(1)=1, a(2)=3, a(3)=10
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array a[0..cache_cap-1]   */
    static size_t  cache_cap = 0; /* allocated size               */
    static idx_t   last_computed = 0; /* largest n computed */

    if (n == 0) return (seq_t)0;  /* not part of the offset */

    /* ensure capacity for index n */
    size_t required = (size_t)(n + 1);
    if (cache == NULL) {
        cache_cap = required;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        /* initialise to zero */
        for (size_t j = 0; j < cache_cap; ++j) cache[j] = (seq_t)0;
    } else if (cache_cap < required) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        /* initialise new elements to zero */
        for (size_t j = cache_cap; j < required; ++j) cache[j] = (seq_t)0;
        cache_cap = required;
    }

    /* compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1)              cache[1] = (seq_t)1;
        else if (i == 2)         cache[2] = (seq_t)3;
        else if (i == 3)         cache[3] = (seq_t)10;
        else {
            seq_t term1 = ((seq_t)(i + 3)) * cache[i - 1];
            seq_t term2 = ((seq_t)(3 * i)) * cache[i - 2];
            seq_t term3 = ((seq_t)(2 * (i - 2))) * cache[i - 3];
            cache[i] = term1 - term2 + term3;
        }
    }
    last_computed = n;
    return cache[n];
}

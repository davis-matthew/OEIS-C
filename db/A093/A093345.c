/*
 * A093345  a(n) = n! * {1 + Sum[i=1..n, 1/i*Sum(j=0..i-1, 1/j!)]}.
 * https://oeis.org/A093345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) - 2*n*a(n-1) + (n^2-2)*a(n-2) -
 *            (n-2)^2*a(n-3) = 0, with a(0)=1, a(1)=2, a(2)=6. This recurrence
 *            is used to compute the sequence a(n). Only integer arithmetic is
 *            employed; the implementation is memoised so that repeated calls
 *            with arbitrary n (in no particular order) are effic...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation table */
    static seq_t *cache = NULL;
    static idx_t allocated = 0;   /* allocated size of cache */
    static idx_t len = 0;         /* number of computed terms */

    /* initialise the cache with the first three terms */
    if (cache == NULL) {
        allocated = 3;
        cache = (seq_t *)malloc(allocated * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)2;   /* a(1) */
        cache[2] = (seq_t)6;   /* a(2) */
        len = 3;
    }

    /* return already computed value */
    if (n < len) return cache[n];

    /* grow the cache if necessary */
    if (n + 1 > allocated) {
        idx_t new_alloc = allocated;
        while (new_alloc <= n) new_alloc <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        allocated = new_alloc;
    }

    /* compute terms up to n using the recurrence */
    for (idx_t k = len; k <= n; ++k) {
        if (k == 0 || k == 1 || k == 2) continue; /* already set */

        seq_t term1 = (seq_t)2 * (seq_t)k * cache[k - 1];
        seq_t term2 = ((seq_t)k * (seq_t)k - (seq_t)2) * cache[k - 2];
        seq_t term3 = ((seq_t)k - (seq_t)2) * ((seq_t)k - (seq_t)2) * cache[k - 3];

        cache[k] = term1 - term2 + term3;
    }

    len = n + 1;
    return cache[n];
}

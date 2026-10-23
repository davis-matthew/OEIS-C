/*
 * A022316  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 0 and a(1) = 11.
 * https://oeis.org/A022316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0) = 0, a(1) = 11
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t used = 0;  /* number of computed entries  */
    static size_t cap  = 0;  /* allocated capacity       */

    /* Initialise the cache on first entry */
    if (!cache) {
        cap   = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)11;
        used = 2;
    }

    /* Grow allocation if necessary */
    if ((idx_t)(cap) <= n) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)(n + 1)) newcap = n + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Compute missing values */
    if ((idx_t)used <= n) {
        idx_t i;
        for (i = (idx_t)used; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + (seq_t)1;
        }
        used = (size_t)(n + 1);
    }

    return cache[n];
}

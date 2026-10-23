/*
 * A140796  a(n)=a(n-1)+6a(n-2), n>2.
 * https://oeis.org/A140796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=14; a(n)=a(n-1)+6*a(n-2) for n>2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation buffer          */
    static size_t len   = 0;      /* number of valid entries      */
    static size_t cap   = 0;      /* allocated capacity           */

    /* Return cached value if already computed. */
    if (n < len)
        return cache[n];

    /* Grow the cache if necessary. */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();           /* insufficient storage */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to index n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)5;
        else if (i == 2)
            cache[i] = (seq_t)14;
        else
            cache[i] = cache[i - 1] + ((seq_t)6) * cache[i - 2];
    }
    len = (size_t)n + 1;

    return cache[n];
}

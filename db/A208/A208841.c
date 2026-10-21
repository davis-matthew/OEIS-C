/*
 * A208841  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A208841
   Formula: a(n) = 2*a(n-1) + 7*a(n-2) - 6*a(n-3)
            with a(1) = 9,  a(2) = 81,  a(3) = 171
   Offset: 1 (a(1) is the first term) */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* static cache state */
    static seq_t *cache   = NULL;   /* array of values a[0..] (a[0] unused)   */
    static idx_t  cache_cap = 0;    /* allocated capacity */
    static idx_t  last_computed = 0;/* highest n already computed */

    /* initialise cache once */
    if (cache == NULL)
    {
        cache_cap = (n > 3) ? (n + 1) : 4;
        cache = alloc_seq_array(cache_cap);
        /* base values */
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)81;
        cache[3] = (seq_t)171;
        last_computed = 3;
    }
    /* enlarge cache if necessary */
    else if (cache_cap <= n)
    {
        seq_t *tmp = alloc_seq_array(n + 1);
        for (idx_t i = 0; i < cache_cap; ++i)
            tmp[i] = cache[i];
        free_seq_array(cache);
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute missing terms */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        if (i <= 3)      /* base values already in place */
            continue;
        seq_t t1 = (seq_t)2 * cache[i - 1];
        seq_t t2 = (seq_t)7 * cache[i - 2];
        seq_t t3 = (seq_t)6 * cache[i - 3];
        cache[i] = t1 + t2 - t3;
    }
    last_computed = n;

    return cache[n];
}

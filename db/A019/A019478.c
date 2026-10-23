/*
 * A019478  a(n) = 5*a(n-1) + a(n-2) - 3*a(n-3).
 * https://oeis.org/A019478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A019478:
   a(0) = 3, a(1) = 15, a(2) = 76
   recurrence: a(n) = 5*a(n-1) + a(n-2) - 3*a(n-3)   (for n >= 3)   */

#include <stdlib.h>

/* Forward declaration of the external integer types.  The build system
   supplies seq_t and idx_t.  */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Memoised linear‑time computation of the recurrence. */
    static seq_t *cache = NULL;   /* cache[0..len-1] holds computed terms   */
    static size_t len  = 0;       /* number of values stored               */
    static size_t cap  = 0;       /* allocated capacity of cache           */

    /* Resolve the index to a size_t for array access.  The sequence
       index is assumed to be small enough to fit in the host size_t. */
    size_t idx = (size_t)n;

    /* Ensure we have computed all terms up to idx. */
    if (len <= idx)
    {
        /* Grow the allocation to hold at least idx+1 terms. */
        size_t needed = idx + 1;
        if (cap < needed)
        {
            size_t new_cap = cap ? cap * 2 : needed;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;   /* allocation failure – caller will see 0 */
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute missing terms. */
        for (size_t i = len; i < needed; ++i)
        {
            if (i == 0)
                cache[i] = (seq_t)3;
            else if (i == 1)
                cache[i] = (seq_t)15;
            else if (i == 2)
                cache[i] = (seq_t)76;
            else
                cache[i] = (5 * cache[i-1]) + cache[i-2] - (3 * cache[i-3]);
        }
        len = needed;
    }

    return cache[idx];
}

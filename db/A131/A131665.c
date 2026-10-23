/*
 * A131665  Sequence is identical to its third differences in absolute value:
 *          a(0), a(1), a(2), a(2n+1)=3a(2n)-3a(2n-1)+2a(2n-2),
 *          a(2n+2)=3a(2n+1)-3a(2n), with a(0)=a(1)=0, a(2)=1.
 * https://oeis.org/A131665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   Recurrence: a(0)=0, a(1)=0, a(2)=1; For n≥3: if (n is even)
 *            a(n)=3*a(n-1)-3*a(n-2); if (n is odd)
 *            a(n)=3*a(n-1)-3*a(n-2)+2*a(n-3); (This is equivalent to
 *            a(n)=3*a(n-2)-3*a(n-4) for n>5)
 * Range:     i8 n <= 15, i16 n <= 34, i32 n <= 75, i64 n <= 158,
 *            i128 n <= 316, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache_len is the number of
       values already computed and stored.  cache points to an
       array that holds at least *cache_len* elements. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of computed terms */

    /* Basic base cases */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* Ensure we have enough storage for the requested index */
    if (cache == NULL)
    {
        /* At least 3 entries are needed for the recurrence */
        size_t cap = (size_t)((n + 1) > 3 ? (n + 1) : 3);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache_len = 0;                   /* nothing yet computed */
    }
    else if (n + 1 > cache_len)
    {
        /* Extend the allocated array if necessary.  We keep the
           previously computed values unchanged. */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failed – abort with zero */
            return 0;
        cache = tmp;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val;
        if (i == 0 || i == 1)
            val = 0;
        else if (i == 2)
            val = 1;
        else if ((i & 1) == 0)           /* even index */
            val = 3 * cache[i - 1] - 3 * cache[i - 2];
        else                              /* odd index */
            val = 3 * cache[i - 1] - 3 * cache[i - 2] + 2 * cache[i - 3];

        cache[i] = val;
    }

    cache_len = n + 1;     /* now all terms up to n are valid */

    return cache[n];
}

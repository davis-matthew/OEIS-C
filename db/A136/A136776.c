/*
 * A136776  Number of primitive multiplex juggling sequences of length n, base
 *          state <1,1> and hand capacity 2.
 * https://oeis.org/A136776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - a(n-3), with a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1 */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;  /* array holding computed terms */
    static size_t cap   = 0;     /* allocated array size (excluding index 0) */
    static size_t len   = 0;     /* largest n that has been computed */

    /* grow the cache if it cannot hold term n */
    if (cap < (size_t)(n + 1))
    {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap < (size_t)(n + 1))
            newcap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!tmp)                        /* allocation failure – return 0 */
            return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* initialise the first three values once */
    if (len == 0)
    {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        len = 3;
    }

    /* return previously computed value if it exists */
    if ((size_t)n <= len)
        return cache[(size_t)n];

    /* compute missing values up to n using the recurrence */
    for (size_t i = len + 1; i <= (size_t)n; ++i)
    {
        cache[i] =
            ( (seq_t)4 * cache[i-1] )
          - ( (seq_t)3 * cache[i-2] )
          -  cache[i-3];
    }
    len = (size_t)n;
    return cache[(size_t)n];
}

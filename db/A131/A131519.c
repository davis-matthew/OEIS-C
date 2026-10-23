/*
 * A131519  a(1) = 1, a(2) = 6, a(3) = 66, a(4) = 714, and a(n) = 11*a(n-1) -
 *          24*a(n-3) for n >= 5.
 * https://oeis.org/A131519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 24*a(n-3) for n>=5; Initial values: a(1)=1,
 *            a(2)=6, a(3)=66, a(4)=714
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sanity: ignore n<1 (offset is 1) */
    if (n == 0)
        return (seq_t)0;

    /* static cache to hold computed terms */
    static seq_t *cache = NULL;     /* a(1) stored at cache[0] */
    static size_t cached = 0;       /* number of terms currently cached */
    static size_t cap = 0;          /* capacity of allocated array */

    /* Ensure array is large enough */
    if (n > cap)
    {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cached + 1; i <= (size_t)n; ++i)
    {
        if (i == 1)
            cache[i - 1] = (seq_t)1;
        else if (i == 2)
            cache[i - 1] = (seq_t)6;
        else if (i == 3)
            cache[i - 1] = (seq_t)66;
        else if (i == 4)
            cache[i - 1] = (seq_t)714;
        else
            cache[i - 1] = (seq_t)11 * cache[i - 2] - (seq_t)24 * cache[i - 4];
    }
    cached = (size_t)n;

    return cache[n - 1];
}

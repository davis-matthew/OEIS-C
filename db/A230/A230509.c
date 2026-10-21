/*
 * A230509  Number of n X 2 0..7 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 8, and upper left element zero.
 * https://oeis.org/A230509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) - 5*a(n-3) + 9*a(n-4) + 12*a(n-5)
 *            with initial values a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=8
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache[0] unused, cache[1..] holds a(n) */
    static size_t cache_size = 0;        /* largest index cached */

    /* initialise cache with the first five terms (offset 1) */
    if (cache == NULL)
    {
        cache_size = 5;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        cache[0] = 0;                    /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)8;
    }

    /* return cached value if already computed */
    if (n <= cache_size)
        return cache[n];

    /* enlarge cache up to index n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));

    /* compute remaining terms using recurrence */
    for (size_t k = cache_size + 1; k <= (size_t)n; ++k)
    {
        if (k <= 5)          /* the first five terms are already set */
            continue;

        seq_t term = 5 * cache[k - 1];   /* 5*a(k-1) */
        term += 9 * cache[k - 4];        /* + 9*a(k-4) */
        term += 12 * cache[k - 5];       /* + 12*a(k-5) */

        seq_t sub = 4 * cache[k - 2];    /* 4*a(k-2) */
        sub += 5 * cache[k - 3];         /* + 5*a(k-3) */

        term -= sub;                     /* subtract the two negative terms */

        cache[k] = term;
    }

    cache_size = (size_t)n;
    return cache[n];
}

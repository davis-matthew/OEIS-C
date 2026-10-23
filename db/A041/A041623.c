/*
 * A041623  Denominators of continued fraction convergents to sqrt(330).
 * https://oeis.org/A041623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 218*a(n-2) - a(n-4) for n>3, with initial values a(0)=1,
 *            a(1)=6, a(2)=217, a(3)=1308
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t    cap   = 0;   /* allocated size */
    static idx_t     last  = 0;   /* highest index computed so far */

    /* Ensure storage for indices 0..n */
    if (cap < (size_t)(n + 1))
    {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            exit(1);            /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Fill the base terms (in case the array was just grown) */
    if (cap >= 1) cache[0] = 1;
    if (cap >= 2) cache[1] = 6;
    if (cap >= 3) cache[2] = 217;
    if (cap >= 4) cache[3] = 1308;

    /* If the requested index is already computed, return it */
    if (n <= last)
        return cache[n];

    /* Compute any missing values using the recurrence */
    idx_t start = last + 1;
    if (start < 4)      /* ensure we use the recurrence only for n>=4 */
        start = 4;

    for (idx_t i = start; i <= n; ++i)
    {
        seq_t prod = (seq_t)218 * cache[i - 2];
        cache[i] = prod - cache[i - 4];
    }

    /* Update the highest computed index */
    last = n;
    return cache[n];
}

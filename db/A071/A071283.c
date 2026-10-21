/*
 * A071283  Numerators of Peirce sequence of order 4.
 * https://oeis.org/A071283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2*a(n-10) - a(n-20) for n > 19 (a(0) ... a(19) are the
 *            given first 20 terms)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of terms currently cached   */
    static size_t cap   = 0;   /* allocated capacity of cache        */

    /* Initialise the first 20 terms once */
    if (cache == NULL)
    {
        /* Start with some capacity – will be grown as needed */
        cap  = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));

        /* Initial 20 values from the OEIS entry */
        const seq_t base[20] = {
            (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)1,
            (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)2, (seq_t)4,
            (seq_t)3, (seq_t)1, (seq_t)5, (seq_t)4, (seq_t)6, (seq_t)3,
            (seq_t)5, (seq_t)7
        };
        for (size_t i = 0; i < 20; ++i)
            cache[i] = base[i];
        len = 20;
    }

    /* If requested index is already cached, return it */
    if (n < len)
        return cache[n];

    /* Compute and extend cache up to n */
    while (len <= n) {
        /* Grow cache if needed */
        if (len >= cap) {
            cap = cap * 2;
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        }

        size_t i = len;
        /* Recurrence: a(i) = 2*a(i-10) - a(i-20) (i >= 20) */
        cache[i] = 2 * cache[i - 10] - cache[i - 20];
        ++len;
    }

    return cache[n];
}

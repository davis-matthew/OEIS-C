/*
 * A053988  Denominators of successive convergents to tan(1/2) using continued
 *          fraction 1/(2-1/(6-1/(10-1/(14-1/(18-1/(22-1/(26-1/30-...))))))).
 * https://oeis.org/A053988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 2*(2*n-1)*a(n-1) – a(n-2) with a(1)=2, a(2)=11. Indices
 *            start at 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), ... */
    static idx_t cache_len = 0;   /* number of elements actually stored */

    if (n == 0) return 0;                 /* not used in tests, but keeps behaviour sane */

    if (cache_len < n)                    /* need to extend the cache up to n */
    {
        size_t new_len = n;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure – suspend */

        /* initialise the first two terms if they do not yet exist */
        if (cache_len < 1) cache[0] = (seq_t)2;          /* a(1) */
        if (cache_len < 2 && n >= 2) cache[1] = (seq_t)11;/* a(2) */

        /* compute any newly required terms */
        for (idx_t i = cache_len; i < new_len; ++i)
        {
            if (i == 0) continue;          /* a(1) already set */
            if (i == 1) continue;          /* a(2) already set */

            /* n = i+1 for the recurrence */
            seq_t coeff = (seq_t)2 * ((seq_t)2 * (seq_t)(i + 1) - (seq_t)1);
            cache[i] = coeff * cache[i - 1] - cache[i - 2];
        }

        cache_len = new_len;               /* remember how many are stored */
    }

    return cache[n - 1];                    /* a(n) */
}

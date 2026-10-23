/*
 * A041687  Denominators of continued fraction convergents to sqrt(363).
 * https://oeis.org/A041687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A041687: Denominators of continued fraction convergents to sqrt(363)
 * Recurrence: a(n) = 724*a(n-2) - a(n-4)    for n > 3
 * Initial values: a(0)=1, a(1)=19, a(2)=723, a(3)=13756
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;  /* array storing a(0)...a(cache_len-1) */
    static idx_t cache_len = 0;    /* number of terms currently stored */

    /* Extend cache if the requested index is beyond the current size */
    if (cache_len <= n)
    {
        /* Allocate or grow the cache */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) /* allocation failed – abort */
            return 0;
        cache = new_cache;

        /* Compute any new terms that are still missing,
           starting after the last computed one */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
                cache[i] = 1;
            else if (i == 1)
                cache[i] = 19;
            else if (i == 2)
                cache[i] = 723;
            else if (i == 3)
                cache[i] = 13756;
            else
                cache[i] = (seq_t)724 * cache[i-2] - cache[i-4];
        }
        cache_len = n + 1;
    }

    return cache[n];
}

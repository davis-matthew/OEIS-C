/*
 * A164265  Partial sums of A162766.
 * https://oeis.org/A164265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2)+7 ; a(1)=4, a(2)=7
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 78, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* highest computed index */

    if (n == 0)                      /* offset is 1, but guard anyway */
        return (seq_t)0;

    /* ensure enough space */
    idx_t need = (n > 2) ? n + 1 : 3;  /* at least indices 1 and 2 */
    if (n > cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();           /* memory allocation failure */
        cache = tmp;

        /* initialise base cases if not already */
        if (cache_size < 2)
        {
            cache[1] = (seq_t)4;
            cache[2] = (seq_t)7;
        }

        /* compute up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            if (i == 1)
                cache[i] = (seq_t)4;
            else if (i == 2)
                cache[i] = (seq_t)7;
            else
                cache[i] = (seq_t)3 * cache[i - 2] + (seq_t)7;
        }
        cache_size = n;
    }

    return cache[n];
}

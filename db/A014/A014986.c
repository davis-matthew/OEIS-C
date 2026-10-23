/*
 * A014986  a(n) = (1 - (-5)^n)/6.
 * https://oeis.org/A014986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-5)^n) / 6
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 28, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=-4, a(n) = -4*a(n-1)+5*a(n-2) for n>=3 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) /* undefined for non‑positive indices */
        return 0;

    /* static cache for memoisation */
    static seq_t *cache        = NULL;
    static idx_t cache_len     = 0;   /* highest index currently cached */

    /* Grow cache if necessary */
    if (n > cache_len)
    {
        idx_t new_len = n;
        void *tmp_ptr = realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (tmp_ptr == NULL) /* allocation failure – return 0 */
            return 0;
        cache = (seq_t *)tmp_ptr;

        /* compute all missing terms */
        for (idx_t i = cache_len + 1; i <= new_len; ++i)
        {
            if (i == 1)
                cache[i] = (seq_t)1;
            else if (i == 2)
                cache[i] = (seq_t)(-4);
            else
            {
                seq_t term1 = cache[i - 1];
                seq_t term2 = cache[i - 2];
                cache[i] = ((seq_t)(-4) * term1) + ((seq_t)5 * term2);
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}

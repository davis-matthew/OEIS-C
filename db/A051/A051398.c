/*
 * A051398  a(n) = -(n-3)*a(n-1) + 2*(n-2)^2.
 * https://oeis.org/A051398
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(n-3)*a(n-1) + 2*(n-2)^2 , offset = 3 a(3) = 2
 * Range:     i8 n <= 8, i16 n <= 11, i32 n <= 15, i64 n <= 23, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                     /* behaviour below offset is irrelevant */

    /* static cache: stores a(3), a(4), ... */
    static seq_t *cache = NULL;        /* array of seq_t */
    static idx_t cache_size = 0;       /* number of entries currently stored (>=0) */

    /* ensure cache can hold values up to n */
    if (n - 3 >= cache_size)
    {
        idx_t needed = (n - 3) + 1;    /* number of entries required */

        /* reallocate with sufficient size */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            return 0;                  /* out of memory – unlikely in normal use */
        cache = tmp;

        /* compute missing entries */
        for (idx_t i = cache_size; i < needed; ++i)
        {
            idx_t current_n = i + 3;   /* actual n value */

            if (current_n == 3)
            {
                cache[i] = (seq_t)2;   /* a(3) = 2 */
            }
            else
            {
                seq_t prev = cache[i - 1];
                seq_t coeff = (seq_t)(current_n - 3);             /* -(n-3) part */
                seq_t term1 = -(coeff * prev);                    /* -(n-3)*a(n-1) */
                seq_t pos   = (seq_t)(current_n - 2);             /* (n-2) */
                seq_t term2 = (seq_t)2 * pos * pos;               /* 2*(n-2)^2 */
                cache[i] = term1 + term2;
            }
        }
        cache_size = needed;
    }

    return cache[n - 3];
}

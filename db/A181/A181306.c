/*
 * A181306  Number of 2-compositions of n having no increasing columns.
 * https://oeis.org/A181306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 3, a(3) = 7 for n >= 4 : a(n) =
 *            2*a(n-1) + 2*a(n-2) - 2*a(n-3) (OEIS A181306)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n  = 0;               /* next free index in cache */

    if (n >= max_n)                        /* need to extend cache */
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failure – terminate */

        /* fill entries that are not yet present */
        for (idx_t i = max_n; i <= n; ++i)
        {
            if (i == 0)      tmp[i] = 1;
            else if (i == 1) tmp[i] = 1;
            else if (i == 2) tmp[i] = 3;
            else if (i == 3) tmp[i] = 7;
            else             tmp[i] = 2 * tmp[i-1] + 2 * tmp[i-2] - 2 * tmp[i-3];
        }

        cache = tmp;
        max_n = new_size;
    }

    return cache[n];
}

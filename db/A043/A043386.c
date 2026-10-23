/*
 * A043386  Numbers having two 4's in base 6.
 * https://oeis.org/A043386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having two 4's in base 6.
   The nth such positive integer, with a(1)=28. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously found terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;    /* allocated capacity */
    static idx_t current = 0;      /* last number examined */

    if (n == 0)  /* offset is 1, below this not defined */
        return (seq_t)0;

    /* extend cache until it contains at least n terms */
    while (cache_size < n)
    {
        /* advance to next candidate */
        current++;

        /* count digits equal to 4 in base 6 */
        idx_t x = current;
        int cnt4 = 0;
        do
        {
            if ((x % 6) == 4)
                cnt4++;
            x /= 6;
        } while (x > 0);

        if (cnt4 == 2)
        {
            /* enlarge cache as needed */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)  /* allocation failure: abort */
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = current;
        }
    }

    return cache[n - 1];
}

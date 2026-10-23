/*
 * A129803  Triangular numbers that are the sum of three consecutive
 *          triangular numbers.
 * https://oeis.org/A129803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=10, a(2)=136, a(n)=14*a(n-1)-a(n-2)-3 for n>=3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of elements stored (index 0 unused) */

    /* Grow cache if necessary */
    if (n + 1 > cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            return 0; /* allocation failed; return 0 as placeholder */

        /* copy existing data */
        if (cache)
        {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }

        /* initialize base cases */
        if (new_size > 0) new_cache[0] = (seq_t)0; /* unused placeholder */
        if (new_size > 1) new_cache[1] = (seq_t)10;
        if (new_size > 2) new_cache[2] = (seq_t)136;

        /* compute new terms using the recurrence */
        seq_t fourteen = (seq_t)14;
        seq_t three   = (seq_t)3;
        for (idx_t i = (cache_size > 2 ? cache_size : 3); i < new_size; ++i)
        {
            new_cache[i] = fourteen * new_cache[i - 1] - new_cache[i - 2] - three;
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}

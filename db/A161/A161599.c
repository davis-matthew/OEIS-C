/*
 * A161599  The list of the B values in the common solutions to the 2
 *          equations 15*k + 1 = A^2, 19*k + 1 = B^2.
 * https://oeis.org/A161599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 17*a(n-1) - a(n-2); a(1)=1, a(2)=18 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) return (seq_t)0;          /* not used, but safe */

    if (n > cache_len)                    /* need to grow cache */
    {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure */

        /* copy existing elements */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* initialise the first terms if they are not already present */
        if (cache_len < 1) new_cache[0] = (seq_t)1;      /* a(1) */
        if (cache_len < 2 && new_len >= 2)
            new_cache[1] = (seq_t)18;                   /* a(2) */

        /* compute the remaining terms using the recurrence */
        idx_t start = (cache_len > 1) ? cache_len : 2;
        for (idx_t i = start; i < new_len; ++i)
            new_cache[i] = new_cache[i-1] * (seq_t)17 - new_cache[i-2];

        free(cache);
        cache     = new_cache;
        cache_len = new_len;
    }

    return cache[n-1];
}

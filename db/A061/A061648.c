/*
 * A061648  Area of all nondecreasing Dyck paths of length 2n.
 * https://oeis.org/A061648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=6, a(3)=29, a(4)=122, a(5)=464, a(6)=1648,
 *            a(7)=5583 * For n ≥ 8 the sequence satisfies: (1) * 10a(n‑1) –
 *            39a(n‑2) + 75a(n‑3) – 75a(n‑4) * + 39a(n‑5) – 10a(n‑6) + a(n‑7)
 *            = 0 * The recurrence follows from the rational generating
 *            function * given in the OEIS entry. The implementation s...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0‑based cache: cache[i] holds a(i); cache[0] = 0 is unused.
       The cache is grown lazily on each call.                     */
    static seq_t *cache = NULL;   /* dynamically allocated array */
    static idx_t cache_size = 0;  /* last index already computed */

    if (n == 0)
        return 0;   /* undefined but harmless for safety */

    /* Grow the cache if necessary */
    if (n > cache_size)
    {
        idx_t new_size = n;
        idx_t old_size = cache_size;
        seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * (new_size + 1));
        if (cache != NULL)
        {
            /* copy existing values */
            for (idx_t i = 0; i <= old_size; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        else
            /* initialise all entries to 0 implicitly by malloc */
            ;

        cache = new_cache;
        cache_size = new_size;

        /* Initialise the first few terms (index 0 unused) */
        if (cache[0] != 0) cache[0] = 0;   /* safety */

        if (cache_size >= 1)  cache[1] = 1;   /* a(1)  */
        if (cache_size >= 2)  cache[2] = 6;   /* a(2)  */
        if (cache_size >= 3)  cache[3] = 29;  /* a(3)  */
        if (cache_size >= 4)  cache[4] = 122; /* a(4)  */
        if (cache_size >= 5)  cache[5] = 464; /* a(5)  */
        if (cache_size >= 6)  cache[6] = 1648;/* a(6)  */
        if (cache_size >= 7)  cache[7] = 5583;/* a(7)  */

        /* Compute any remaining terms up to the new size */
        for (idx_t i = old_size + 1; i <= cache_size; ++i)
        {
            if (i <= 7) /* already set above */
                continue;
            /* Recurrence (1) for i ≥ 8 */
            cache[i] =
                 10 * cache[i-1]
               - 39 * cache[i-2]
               + 75 * cache[i-3]
               - 75 * cache[i-4]
               + 39 * cache[i-5]
               - 10 * cache[i-6]
               +  cache[i-7];
        }
    }

    return cache[n];
}

/*
 * A026028  Expansion of 1/((1-2x)(1-5x)(1-9x)(1-11x)).
 * https://oeis.org/A026028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 27*a(n-1)-249*a(n-2)+893*a(n-3)-990*a(n-4),
   a(0)=1,a(1)=27,a(2)=480,a(3)=7130 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store already computed terms. */
    static seq_t *cache      = NULL; /* array of terms */
    static size_t  cache_len = 0;     /* allocated length */
    static seq_t  pow2, pow5, pow9, pow11; /* powers for current index */
    static seq_t  f2,    f3,    f4;   /* intermediate convolutions */
    static idx_t  last_n   = 0;        /* highest computed index */

    /* First call: initialise everything for n = 0. */
    if (cache == NULL)
    {
        cache_len = 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failure – unlikely */

        pow2 = pow5 = pow9 = pow11 = (seq_t)1;
        f2   = f3   = f4   = (seq_t)1;  /* a(0) */
        cache[0] = (seq_t)1;
        last_n   = 0;
    }

    /* If the requested term is already in the cache, return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate up to index n. */
    size_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;            /* allocation failure – unlikely */

    cache     = tmp;
    cache_len = new_len;

    /* Extend the cache from last_n+1 up to n. */
    for (idx_t i = last_n + 1; i <= n; ++i)
    {
        /* Update powers for the new index. */
        pow2  = (seq_t)((unsigned __int128)pow2  * 2);
        pow5  = (seq_t)((unsigned __int128)pow5  * 5);
        pow9  = (seq_t)((unsigned __int128)pow9  * 9);
        pow11 = (seq_t)((unsigned __int128)pow11 * 11);

        /* Convolutions for the four factors. */
        seq_t new_f2 = (seq_t)((unsigned __int128)5 * f2 + pow2);
        seq_t new_f3 = (seq_t)((unsigned __int128)9 * f3 + new_f2);
        seq_t new_f4 = (seq_t)((unsigned __int128)11 * f4 + new_f3);

        /* Store the resulting coefficient and shift state. */
        cache[i] = new_f4;
        f2 = new_f2;
        f3 = new_f3;
        f4 = new_f4;
    }

    last_n = n;
    return cache[n];
}

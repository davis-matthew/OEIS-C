/*
 * A159287  Expansion of x^2/(1-x^2-2*x^3).
 * https://oeis.org/A159287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 57, u64 n <= 109,
 *            u128 n <= 215, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(1)=0, a(2)=1, a(n)=a(n-2)+2*a(n-3) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_comp = 2;          /* highest index computed so far */

    /* initial allocation */
    if (cache_size == 0)
    {
        cache_size = 3;
#ifdef __cplusplus
        cache = new seq_t[cache_size];
#else
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
#endif
        if (!cache) return 0;            /* allocation failed */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        last_comp = 2;
    }

    /* need to compute higher indices? */
    if (n > last_comp)
    {
        /* enlarge buffer if necessary */
        if ((idx_t)n + 1 > cache_size)
        {
            idx_t new_size = cache_size;
            while (new_size <= n) new_size <<= 1;
            seq_t *tmp;
#ifdef __cplusplus
            tmp = new seq_t[new_size];
            for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
            delete[] cache;
#else
            tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#endif
            if (!tmp) return 0;          /* allocation failed */
            cache = tmp;
            cache_size = new_size;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = last_comp + 1; i <= n; ++i)
            cache[i] = cache[i-2] + ((seq_t)2 * cache[i-3]);

        last_comp = n;
    }

    return cache[n];
}

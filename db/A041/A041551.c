/*
 * A041551  Denominators of continued fraction convergents to sqrt(293).
 * https://oeis.org/A041551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A041551: Denominators of continued fraction convergents to sqrt(293).
   Recurrence: a(n) = 4964*a(n-5) + a(n-10)  for n > 9
   Initial values: a(0)=1, a(1)=8, a(2)=9, a(3)=17, a(4)=145,
                   a(5)=4947, a(6)=39721, a(7)=44668, a(8)=84389, a(9)=719780
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache  = NULL;
    static idx_t cache_len = 0;          /* number of terms cached (cache[0..cache_len-1]) */
    static const seq_t init[10] = {
        (seq_t)1,
        (seq_t)8,
        (seq_t)9,
        (seq_t)17,
        (seq_t)145,
        (seq_t)4947,
        (seq_t)39721,
        (seq_t)44668,
        (seq_t)84389,
        (seq_t)719780
    };

    /* If we have never allocated the cache, do it now and fill
       from the beginning up to n (or up to 9 if n<10).            */
    if (cache == NULL)
    {
        cache_len = (idx_t)(n + 1);          /* worst‑case needed length */
        cache     = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure – should not happen */

        /* fill terms that we already know */
        for (idx_t i = 0; i < cache_len; ++i)
        {
            if (i < 10)
                cache[i] = init[i];
            else
            {
                seq_t t = cache[i-5];
                t = t * (seq_t)4964;
                t = t + cache[i-10];
                cache[i] = t;
            }
        }
        return cache[n];
    }

    /* If the requested index is beyond the current cache, extend it. */
    if (n + 1 > cache_len)
    {
        idx_t new_len   = n + 1;
        seq_t *new_ptr  = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (new_ptr == NULL) return (seq_t)0;   /* reallocation failed */

        /* Reuse the previously cached portion */
        cache = new_ptr;

        /* Compute the new entries */
        for (idx_t i = cache_len; i < new_len; ++i)
        {
            if (i < 10)
                cache[i] = init[i];
            else
            {
                seq_t t = cache[i-5];
                t = t * (seq_t)4964;
                t = t + cache[i-10];
                cache[i] = t;
            }
        }
        cache_len = new_len;
    }

    /* Now cache[n] is guaranteed to exist */
    return cache[n];
}

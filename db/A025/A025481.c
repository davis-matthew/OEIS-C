/*
 * A025481  a(0) = 0; a(n) = a(n/2)/2 if n = a(n/2) = 0 (mod 2); a(n) =
 *          a(n-1)+1 otherwise.
 * https://oeis.org/A025481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(n)=a(n/2)/2 if n even and a(n/2) even; 
   a(n)=a(n-1)+1 otherwise. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_max = 0;

    /* Ensure cache[0] exists and is initialised */
    if (cache == NULL)
    {
        /* allocate for at least 1 element */
        cache_size = (n >= 0) ? (n + 1) : 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return (seq_t)0; /* fallback, unlikely */
        cache[0] = (seq_t)0;
        cache_max = 0;
    }
    else if (n + 1 > cache_size)
    {
        /* grow cache to accommodate n */
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1; /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) return (seq_t)0; /* fallback */
        cache = tmp;
        cache_size = new_size;
    }

    if (n <= cache_max)
        return cache[n];

    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        seq_t half_val = cache[i / 2];
        if ( ( (i & (idx_t)1) == 0 ) && ( (half_val & (seq_t)1) == 0 ) )
            cache[i] = half_val / (seq_t)2;
        else
            cache[i] = cache[i - 1] + (seq_t)1;
    }

    cache_max = n;
    return cache[n];
}

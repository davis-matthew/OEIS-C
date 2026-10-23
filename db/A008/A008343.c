/*
 * A008343  a(1)=1; thereafter a(n+1) = a(n)-n if a(n) >= n otherwise a(n+1) =
 *          a(n)+n.
 * https://oeis.org/A008343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1 + a(n-1)) mod (2*(n-1))
 * Range:     u8 n <= 121, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* initial value: a(1) = 1 */
/* recurrence: a(n+1) = a(n)-n if a(n) >= n else a(n)+n   */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: index 1.. */
    static seq_t *cache = NULL;
    static idx_t  cache_size   = 0;
    static idx_t  cache_capacity = 0;

    if (n == 0)
        return 0;   /* undefined, but return 0 for safety */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_capacity = 16;                     /* arbitrary start size */
        cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
        if (!cache)
            exit(1);                               /* fatal allocation error */
        cache[1] = (seq_t)1;                      /* a(1) = 1 */
        cache_size = 1;
    }

    /* grow cache capacity if needed */
    if (n > cache_size) {
        if (n + 1 > cache_capacity) {
            size_t newcap = (size_t)cache_capacity;
            while (newcap <= (size_t)n) newcap <<= 1;
            seq_t *newc = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!newc)
                exit(1);                           /* fatal allocation error */
            cache = newc;
            cache_capacity = (idx_t)newcap;
        }

        /* compute missing values */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t im1  = (seq_t)(i - 1);
            seq_t denom = im1 * (seq_t)2;          /* 2*(i-1) */
            seq_t num   = prev + im1;              /* a(i-1) + (i-1) */
            cache[i] = num % denom;                /* modulo */
        }
        cache_size = n;
    }

    return cache[n];
}

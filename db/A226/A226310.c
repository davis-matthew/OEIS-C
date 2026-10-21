/*
 * A226310  a(n+5) = a(n+4)+a(n+3)+a(n+2)+a(n+1)+2*a(n) with a(0)=0,
 *          a(1)=a(2)=a(3)=a(4)=1.
 * https://oeis.org/A226310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+5) = a(n+4)+a(n+3)+a(n+2)+a(n+1)+2*a(n)
   with a(0)=0, a(1)=a(2)=a(3)=a(4)=1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to remember previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Extend the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* Initialize any newly added elements */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = 0;
            } else if (i <= 4) {
                cache[i] = 1;
            } else {
                seq_t val = cache[i-1];
                val += cache[i-2];
                val += cache[i-3];
                val += cache[i-4];
                val += cache[i-5];
                val += cache[i-5];              /* 2 * a(i-5) */
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}

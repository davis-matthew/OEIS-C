/*
 * A063099  Dimension of the space of weight 2n cusp forms for Gamma_0( 31 ).
 * https://oeis.org/A063099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4), with a(1)=2, a(2)=7, a(3)=13,
 *            a(4)=17, a(5)=23 (for n>5)
 * Range:     u8 n <= 48, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* The value 0 is never returned for n>=1; it is only used
       as a placeholder when the array is freshly allocated. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest index currently stored */

    if (n == 0) return 0;          /* undefined for n=0, but guard */

    /* Extend cache if necessary */
    if (n > cache_size) {
        idx_t new_size = n;        /* we need indices up to n inclusive */
        size_t alloc_bytes = ((size_t)new_size + 1) * sizeof(seq_t);
        seq_t *new_cache = (seq_t *)malloc(alloc_bytes);
        if (!new_cache) return 0;  /* out of memory: return 0 (unlikely) */

        /* Copy old data if any */
        if (cache) {
            memcpy(new_cache, cache, ((size_t)cache_size + 1) * sizeof(seq_t));
            free(cache);
        } else {
            /* initialise index 0 slot to 0 for safety */
            new_cache[0] = 0;
        }

        /* Fill new entries */
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            if (i <= 5) {            /* initial conditions */
                switch (i) {
                    case 1: new_cache[i] = 2; break;
                    case 2: new_cache[i] = 7; break;
                    case 3: new_cache[i] = 13; break;
                    case 4: new_cache[i] = 17; break;
                    case 5: new_cache[i] = 23; break;
                }
            } else {                  /* recurrence for n>5 */
                new_cache[i] = new_cache[i - 1] + new_cache[i - 3] - new_cache[i - 4];
            }
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}

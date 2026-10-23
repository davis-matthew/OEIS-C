/*
 * A131887  Number of Khalimsky-continuous functions with a three-point
 *          codomain.
 * https://oeis.org/A131887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131887
   Recurrence:
     a(1) = 3, a(2) = 5, a(3) = 11, a(4) = 19
     a(2k)   = a(2k-1) + a(2k-2) + a(2k-3)
     a(2k-1) = a(2k-2) + 2 * a(2k-3)
   The function caches computed terms and works for any call order.
*/

#include <stdlib.h>
#include <string.h>   /* for memcpy */

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* highest index already computed */

    /* first time: allocate enough space for at least the first 4 terms */
    if (!cache) {
        idx_t size = (n > 4 ? n : 4);        /* allocate up to max(n,4) */
        cache = (seq_t *)malloc(sizeof(seq_t) * (size + 1));
        if (!cache) return (seq_t)0;         /* allocation failed, return 0 */

        /* base values */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)19;
        cached   = 4;
    }

    /* expand cache if needed */
    if (n > cached) {
        /* reallocate larger array */
        idx_t newsize = n;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (newsize + 1));
        if (!new_cache) return (seq_t)0;      /* allocation failed, return 0 */

        memcpy(new_cache, cache, sizeof(seq_t) * (cached + 1));
        free(cache);
        cache = new_cache;

        /* compute new terms iteratively */
        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i & 1) {   /* odd i: a(2k-1) = a(i-1) + 2*a(i-2) */
                cache[i] = cache[i - 1] + ((seq_t)2 * cache[i - 2]);
            } else {       /* even i: a(2k)   = a(i-1) + a(i-2) + a(i-3) */
                cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
            }
        }
        cached = n;
    }

    return cache[n];
}

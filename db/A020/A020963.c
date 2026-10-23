/*
 * A020963  a(n) = Sum_{k>=1} floor( 2*(1+sqrt(2))^(n-k) ).
 * https://oeis.org/A020963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} floor( 2*(1+sqrt(2))^(n-k) ) Recurrence for
 *            n>5: a(n)=3*a(n-1)-4*a(n-3)+a(n-4)+a(n-5) Initial values:
 *            a(1)=2, a(2)=6, a(3)=17, a(4)=45, a(5)=112
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>               /* abort, size_t */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based indices; cache[0] unused */
    static size_t cap   = 0;      /* highest index cached so far */

    if (n == 0)
        return 0;                 /* offset starts at 1 */

    size_t idx = (size_t)n;
    if (idx > cap) {
        /* Allocate a new array large enough for all indices up to idx */
        seq_t *new_cache = alloc_seq_array(idx + 1);   /* indices 0..idx */
        if (!new_cache)
            abort();           /* allocation failed */

        /* Copy previously cached terms (if any) */
        for (size_t i = 1; i <= cap; ++i)
            new_cache[i] = cache[i];

        /* Compute missing terms */
        for (size_t i = cap + 1; i <= idx; ++i) {
            switch ((int)i) {
            case 1: new_cache[1] = 2;  break;
            case 2: new_cache[2] = 6;  break;
            case 3: new_cache[3] = 17; break;
            case 4: new_cache[4] = 45; break;
            case 5: new_cache[5] = 112;break;
            default: {
                /* a(i) = 3*a(i-1) - 4*a(i-3) + a(i-4) + a(i-5) */
                seq_t sum = 3 * new_cache[i - 1]
                          + new_cache[i - 4]
                          + new_cache[i - 5];
                seq_t sub = 4 * new_cache[i - 3];
                new_cache[i] = sum - sub;
                break;
            }
            }
        }
        /* Free the old cache and replace it */
        if (cache) {
            free_seq_array(cache);
        }
        cache = new_cache;
        cap   = idx;
    }

    return cache[idx];
}

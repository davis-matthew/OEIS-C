/*
 * A038342  G.f.: 1/(1 - 3 x - 3 x^2 + 4 x^3 + x^4 - x^5).
 * https://oeis.org/A038342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+3*a(n-2)-4*a(n-3)-a(n-4)+a(n-5) with a(0)=1,
 *            a(1)=3, a(2)=12, a(3)=41, a(4)=146
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of precomputed terms */

    /* Compute up to n if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)                /* allocation failed */
            return 0;
        cache = tmp;

        for (idx_t i = cache_size; i < new_size; ++i) {
            switch (i) {
                case 0: cache[i] = 1; break;
                case 1: cache[i] = 3; break;
                case 2: cache[i] = 12; break;
                case 3: cache[i] = 41; break;
                case 4: cache[i] = 146; break;
                default:
                    cache[i] = 3 * cache[i - 1]
                             + 3 * cache[i - 2]
                             - 4 * cache[i - 3]
                             -   cache[i - 4]
                             +   cache[i - 5];
                    break;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}

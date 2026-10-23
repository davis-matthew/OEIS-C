/*
 * A145655  Partial sums of A080674.
 * https://oeis.org/A145655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(4^(n+1)-3*n-4)/9
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=4, a(2)=24, a(3)=108
   a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3)  (n>=4)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined: offset starts at 1 */

    static seq_t *cache = NULL;     /* cached values */
    static idx_t cache_len = 0;     /* number of cached entries */
    static idx_t cache_cap = 0;     /* allocated capacity */

    if (n > cache_len) {
        /* ensure capacity */
        if (n > cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap : 4;
            while (new_cap < n)
                new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)4;          /* a(1) */
            } else if (i == 1) {
                cache[i] = (seq_t)24;         /* a(2) */
            } else if (i == 2) {
                cache[i] = (seq_t)108;        /* a(3) */
            } else {
                /* a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3) */
                seq_t t1 = cache[i - 1];
                seq_t t2 = cache[i - 2];
                seq_t t3 = cache[i - 3];
                cache[i] = 6 * t1 - 9 * t2 + 4 * t3;
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}

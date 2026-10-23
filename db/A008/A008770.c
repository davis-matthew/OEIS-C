/*
 * A008770  Expansion of (1+x^9)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A008770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) - a(n-5) + a(n-6) -
 *            a(n-7) + 2*a(n-8) - a(n-9) with a(0)=1, a(1)=1, a(2)=2, a(3)=3,
 *            a(4)=5, a(5)=6, a(6)=9, a(7)=11, a(8)=15
 * Range:     u8 n <= 25, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;            /* number of entries stored */

    size_t idx = (size_t)n;
    if (idx + 1 > cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size != 0) return 0; /* allocation failure */
        cache = tmp;
        size_t old_size = cache_size;
        for (size_t i = old_size; i < new_size; ++i) {
            if (i < 9) {                     /* initial terms */
                switch ((int)i) {
                    case 0: cache[i] = (seq_t)1; break;
                    case 1: cache[i] = (seq_t)1; break;
                    case 2: cache[i] = (seq_t)2; break;
                    case 3: cache[i] = (seq_t)3; break;
                    case 4: cache[i] = (seq_t)5; break;
                    case 5: cache[i] = (seq_t)6; break;
                    case 6: cache[i] = (seq_t)9; break;
                    case 7: cache[i] = (seq_t)11; break;
                    case 8: cache[i] = (seq_t)15; break;
                }
            } else {                         /* recurrence for i >= 9 */
                cache[i] =
                    (seq_t)2 * cache[i-1]
                    - cache[i-2]
                    + cache[i-3]
                    - cache[i-4]
                    - cache[i-5]
                    + cache[i-6]
                    - cache[i-7]
                    + (seq_t)2 * cache[i-8]
                    - cache[i-9];
            }
        }
        cache_size = new_size;
    }
    return cache[idx];
}

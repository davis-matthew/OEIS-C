/*
 * A117474  The values of 'a' in a^2 + b^2 = c^2 where b - a = 7 and
 *          gcd(a,b,c)=1.
 * https://oeis.org/A117474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) + 14 with initial values a(1)=5,
 *            a(2)=8, a(3)=48, a(4)=65
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = (seq_t *)0;
    static idx_t cache_size = 0;

    if (n <= 0) return 0; /* undefined for n<1, return 0 */

    /* extend cache if needed */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure, return 0 */
        cache = new_cache;

        /* fill newly allocated entries */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t val = 0;
            switch (i) {
                case 1: val = (seq_t)5; break;
                case 2: val = (seq_t)8; break;
                case 3: val = (seq_t)48; break;
                case 4: val = (seq_t)65; break;
                default:
                    /* recurrence uses values i-2 and i-4 */
                    val = (seq_t)6 * cache[i-2] - cache[i-4] + (seq_t)14;
                    break;
            }
            cache[i] = val;
        }
        cache_size = n;
    }
    return cache[n];
}

/*
 * A188701  Number of n X 4 binary arrays without the pattern 0 0 diagonally
 *          or vertically.
 * https://oeis.org/A188701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 20*a(n-2) + 27*a(n-3) - 14*a(n-4) * - 25*a(n-5)
 *            + 4*a(n-6) + 5*a(n-7) - a(n-8); * Initial values: a(1)=16,
 *            a(2)=55, a(3)=387, a(4)=1876, a(5)=10857, * a(6)=57665,
 *            a(7)=318732, a(8)=1729531
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* offset 1, undefined for n<1 */

    static idx_t cache_size = 0;
    static seq_t *cache = NULL;

    if (cache_size < n + 1) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */

        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            switch (i) {
                case 1: cache[i] = 16; break;
                case 2: cache[i] = 55; break;
                case 3: cache[i] = 387; break;
                case 4: cache[i] = 1876; break;
                case 5: cache[i] = 10857; break;
                case 6: cache[i] = 57665; break;
                case 7: cache[i] = 318732; break;
                case 8: cache[i] = 1729531; break;
                default:
                    cache[i] = cache[i-1]
                           + ((seq_t)20) * cache[i-2]
                           + ((seq_t)27) * cache[i-3]
                           - ((seq_t)14) * cache[i-4]
                           - ((seq_t)25) * cache[i-5]
                           + ((seq_t)4)  * cache[i-6]
                           + ((seq_t)5)  * cache[i-7]
                           - cache[i-8];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}

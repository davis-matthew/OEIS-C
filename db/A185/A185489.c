/*
 * A185489  Number of (n+1) X 2 binary arrays with every 2 X 2 subblock sum
 *          equal to some horizontal or vertical neighbor 2 X 2 subblock sum.
 * https://oeis.org/A185489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+3*a(n-2)-14*a(n-3)+24*a(n-4)-16*a(n-5)-24*a(n-6)+16*a(n-7)-64*a(n-8)
 *            (n ≥ 9), with initial values a(1)=0, a(2)=24, a(3)=36, a(4)=160,
 *            a(5)=380, a(6)=1332, a(7)=3616, a(8)=11828
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache: cache[i] holds a(i) for i>=1.
       We keep space for index 0 (unused) so that indices match
       1‑based OEIS indexing. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 1;  /* currently allocated slots (≥1) */

    if (n < 1)
        return 0;  /* behaviour for n < 1 is unspecified */

    /* Resize cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();  /* allocation failed */
        cache = tmp;
        cache[0] = 0;   /* unused slot */

        /* Compute new entries */
        for (idx_t i = cache_cap; i <= n; ++i) {
            switch (i) {
                case 1:  cache[i] = 0;     break;
                case 2:  cache[i] = 24;    break;
                case 3:  cache[i] = 36;    break;
                case 4:  cache[i] = 160;   break;
                case 5:  cache[i] = 380;   break;
                case 6:  cache[i] = 1332;  break;
                case 7:  cache[i] = 3616;  break;
                case 8:  cache[i] = 11828; break;
                default:
                    cache[i] =
                        3 * cache[i-1] + 3 * cache[i-2]
                      - 14 * cache[i-3] + 24 * cache[i-4]
                      - 16 * cache[i-5] - 24 * cache[i-6]
                      + 16 * cache[i-7] - 64 * cache[i-8];
                    break;
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}

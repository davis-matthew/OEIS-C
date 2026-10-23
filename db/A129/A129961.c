/*
 * A129961  Main diagonal of triangular array T: T(j,1) = 1 for ((j-1) mod 8)
 *          < 4, else 0; T(j,k) = T(j-1,k-1)+T(j,k-1) for 2 <= k <= j.
 * https://oeis.org/A129961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)-14*a(n-2)+16*a(n-3)-10*a(n-4)+4*a(n-5) with
 *            a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=15
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for offset 1; return 0 to avoid UB */
        return 0;

    /* static cache for already computed terms */
    static seq_t *cache = NULL;     /* array of a(1)..a(max_cached) */
    static idx_t  cache_cap = 0;    /* allocated capacity (terms) */
    static idx_t  cache_size = 0;   /* highest index computed */

    /* enlarge cache if necessary */
    if (n > cache_size) {
        idx_t newcap = cache_cap ? cache_cap : 8;
        while (newcap < n)
            newcap <<= 1;

        cache = (seq_t *)realloc(cache, (size_t)newcap * sizeof(seq_t));
        cache_cap = newcap;

        /* compute missing terms up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache[0] = 1; break;
                case 2: cache[1] = 2; break;
                case 3: cache[2] = 4; break;
                case 4: cache[3] = 8; break;
                case 5: cache[4] = 15; break;
                default: {
                    seq_t a1 = cache[i-2]; /* a(i-1) */
                    seq_t a2 = cache[i-3]; /* a(i-2) */
                    seq_t a3 = cache[i-4]; /* a(i-3) */
                    seq_t a4 = cache[i-5]; /* a(i-4) */
                    seq_t a5 = cache[i-6]; /* a(i-5) */
                    cache[i-1] = 6*a1 - 14*a2 + 16*a3 - 10*a4 + 4*a5;
                }
            }
        }
        cache_size = n;
    }

    return cache[n-1];
}

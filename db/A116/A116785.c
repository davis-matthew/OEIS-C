/*
 * A116785  Number of permutations of length n which avoid the patterns 2143,
 *          2341, 4213.
 * https://oeis.org/A116785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Recurrence of order 8 derived from the generating function a(n)
 *            = 10*a(n-1) - 42*a(n-2) + 97*a(n-3) -135*a(n-4) +116*a(n-5)
 *            -60*a(n-6) +17*a(n-7) -2*a(n-8) Initial values (offset 1):
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=71, a(6)=222, a(7)=652,
 *            a(8)=1838
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t computed = 0;

    /* Ensure cache has room for indices up to n */
    size_t needed = (size_t)n + 1;            /* index 0 .. n inclusive   */
    if (cap < needed) {
        size_t newcap = cap ? cap << 1 : 16;
        if (newcap < needed) newcap = needed;
        seq_t *newc = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (cache) {
            for (size_t i = 0; i < cap; ++i) newc[i] = cache[i];
            free(cache);
        }
        cache = newc;
        cap   = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        if (k <= 8) {               /* initial values */
            switch (k) {
                case 1: cache[1] = (seq_t)1;  break;
                case 2: cache[2] = (seq_t)2;  break;
                case 3: cache[3] = (seq_t)6;  break;
                case 4: cache[4] = (seq_t)21; break;
                case 5: cache[5] = (seq_t)71; break;
                case 6: cache[6] = (seq_t)222;break;
                case 7: cache[7] = (seq_t)652;break;
                case 8: cache[8] = (seq_t)1838;break;
            }
        } else {                    /* recurrence */
            seq_t pos = (seq_t)0;
            pos += mul_small(cache[k-1], 10);
            pos += mul_small(cache[k-3], 97);
            pos += mul_small(cache[k-5], 116);
            pos += mul_small(cache[k-7], 17);

            seq_t neg = (seq_t)0;
            neg += mul_small(cache[k-2], 42);
            neg += mul_small(cache[k-4], 135);
            neg += mul_small(cache[k-6], 60);
            neg += mul_small(cache[k-8], 2);

            cache[k] = pos - neg;   /* pos > neg for all n */
        }
    }
    computed = (size_t)n;
    return cache[n];
}

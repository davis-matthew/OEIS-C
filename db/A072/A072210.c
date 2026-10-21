/*
 * A072210  a(1)=a(2)=1; a(n)=reverse(reverse(a(n-1))+reverse(a(n-2))) for n >
 *          2.
 * https://oeis.org/A072210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(reverse(a(n-1)) + reverse(a(n-2))) for n > 2,
 *            with a(1) = a(2) = 1
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 48, u64 n <= 95, u128 n <= 185,
 *            big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    /* Static cache of computed values (1‑based index).  */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* largest n already computed */

    /* Grow cache if needed. */
    if (n > cache_len) {
        seq_t *newcache;
        newcache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!newcache) {
            /* Out of memory – fall back to recomputing all values */
            cache = NULL;
            cache_len = 0;
            newcache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
            if (!newcache) return (seq_t)0; /* cannot allocate */
        }
        cache = newcache;

        /* Compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1 || i == 2) {
                cache[i - 1] = (seq_t)1;
            } else {
                seq_t rev_prev    = reverse(cache[i - 2]); /* a(i-1) */
                seq_t rev_prev2   = reverse(cache[i - 3]); /* a(i-2) */
                seq_t sum         = rev_prev + rev_prev2;
                cache[i - 1]      = reverse(sum);
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}

/*
 * A099187  Let D(n) = n*(9*n^2-9*n+2)/2 then a(k+1) = D(a(k)) with a(1) = 1
 *          and a(2) = D(2).
 * https://oeis.org/A099187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   D(n) = n*(9*n^2-9*n+2)/2; a(1) = 1, a(2) = D(2), a(k+1) =
 *            D(a(k)) for k >= 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   D
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/D.h"

/* Return the nth term of the sequence, with external linkage. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* offset is 1 */

    /* static cache of computed values, 1‑based indexing */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_high = 0;           /* highest index computed */

    /* allocate or grow the cache to hold n+1 elements (index 0 unused) */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure */
        memset(cache, 0, cache_cap * sizeof(seq_t));
    } else if ((size_t)n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure */
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_high + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = 1;                     /* a(1) */
        } else if (i == 2) {
            cache[i] = D((seq_t)2);           /* a(2) = D(2) */
        } else {
            cache[i] = D(cache[i - 1]);       /* recurrence */
        }
    }
    cache_high = n;
    return cache[n];
}

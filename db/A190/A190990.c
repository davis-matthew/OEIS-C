/*
 * A190990  a(n) = 10*a(n-1) - 8*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 8*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for dynamic programming */
    static seq_t *cache = NULL;
    static idx_t sz = 0;      /* number of entries already computed */
    static idx_t cap = 0;     /* allocated capacity */

    /* Ensure a[0] and a[1] are available for all n>=0 */
    if (sz == 0) {
        /* Allocate space for at least two elements */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * 2);
        cap = 2;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        sz = 2;
    }

    /* If we need more terms, extend the cache and compute them */
    if (n >= sz) {
        idx_t needed = n + 1;
        if (needed > cap) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
            cap = needed;
        }
        for (idx_t i = sz; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
                continue;
            }
            if (i == 1) {
                cache[1] = (seq_t)1;
                continue;
            }
            cache[i] = (seq_t)10 * cache[i - 1] - (seq_t)8 * cache[i - 2];
        }
        sz = n + 1;
    }

    return cache[n];
}

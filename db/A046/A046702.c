/*
 * A046702  a(n)=a(n-a(n-1))+a(n-1-a(n-2))+a(n-2-a(n-3)), n>3.
 *          a(1)=a(2)=a(3)=1.
 * https://oeis.org/A046702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(n-a(n-1)) + a(n-1-a(n-2)) + a(n-2-a(n-3)), with
 *            a(1)=a(2)=a(3)=1. A memoised iterative implementation
 * Range:     u8 n <= 378, u16 n <= 8226, u32 n <= 8226, u64 n <= 8226,
 *            u128 n <= 8226, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache holding all computed terms; index 0 is unused */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */
    static idx_t last = 0;   /* largest index already computed */

    /* grow cache if needed */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;   /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed: cannot continue */
            abort();
        }
        /* zero out any new elements to avoid garbage reads */
        if (new_cap > cap) {
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute all missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = 1;
        } else {
            idx_t d1 = (idx_t)cache[i - 1];
            idx_t d2 = (idx_t)cache[i - 2];
            idx_t d3 = (idx_t)cache[i - 3];
            idx_t idx1 = i - d1;
            if (idx1 < 1) idx1 = 1;        /* safety guard */
            idx_t idx2 = (i - 1) - d2;
            if (idx2 < 1) idx2 = 1;
            idx_t idx3 = (i - 2) - d3;
            if (idx3 < 1) idx3 = 1;
            cache[i] = cache[idx1] + cache[idx2] + cache[idx3];
        }
    }
    last = n;
    return cache[n];
}

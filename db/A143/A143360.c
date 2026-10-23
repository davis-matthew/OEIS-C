/*
 * A143360  Sum of root degrees of all symmetric ordered trees with n edges.
 * https://oeis.org/A143360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   2*(n+3)*a(n) + (-n-5)*a(n-1) + (-11*n-3)*a(n-2) +
 *            2*(2*n+1)*a(n-3) +12*(n-3)*a(n-4)=0 with initial values a(1)=1,
 *            a(2)=3, a(3)=5, a(4)=12
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 27, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values for n = 1 .. 4 */
    static const seq_t base[4] = { (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)12 };

    /* cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t max_n = 0;          /* largest n for which cache[n-1] is set */

    if (n == 0) {                     /* offset of the sequence is 1 */
        return (seq_t)0;
    }

    if (n <= 4) {                     /* return known initial values */
        return base[(size_t)(n - 1)];
    }

    /* initialize the cache with the first four terms if not yet done */
    if (max_n < 4) {
        size_t need = 4;
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache_size = need;
        cache[0] = base[0];
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        max_n = 4;
    }

    /* extend the cache up to n if necessary */
    if ((size_t)n > max_n) {
        if ((size_t)n > cache_size) {
            cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
            cache_size = (size_t)n;
        }
        for (idx_t i = (idx_t)(max_n + 1); i <= n; ++i) {
            size_t idx = (size_t)(i - 1);
            seq_t a1 = cache[idx - 1];   /* a(i-1) */
            seq_t a2 = cache[idx - 2];   /* a(i-2) */
            seq_t a3 = cache[idx - 3];   /* a(i-3) */
            seq_t a4 = cache[idx - 4];   /* a(i-4) */

            /* numerator of recurrence: (i+5)*a1 + (11*i+3)*a2
               - 2*(2*i+1)*a3 - 12*(i-3)*a4 */
            seq_t num = (seq_t)(i + 5) * a1
                       + (seq_t)(11 * i + 3) * a2
                       - (seq_t)(2 * (2 * i + 1)) * a3
                       - (seq_t)(12 * (i - 3)) * a4;

            seq_t denom = (seq_t)(2 * (i + 3));
            cache[idx] = num / denom;   /* integer division (exact) */
        }
        max_n = (size_t)n;
    }

    return cache[(size_t)(n - 1)];
}

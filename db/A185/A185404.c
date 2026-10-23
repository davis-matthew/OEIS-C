/*
 * A185404  a(n) = C(2n,n) * (7^n/n!^2) * Product_{k=0..n-1} (7k+3)*(7k+4).
 * https://oeis.org/A185404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n,n) * (7^n / n!^2) * prod_{k=0}^{n-1} (7k+3)(7k+4)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_len   = 0;
    static size_t computed    = 0;   /* number of already computed terms */

    if (n == (idx_t)0)
        return (seq_t)1;

    /* allocate / grow cache */
    size_t needed = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        cache_len = needed;
    } else if (needed > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cache = tmp;
        cache_len = needed;
    }

    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    for (idx_t k = computed; k <= n; ++k) {
        seq_t cur   = cache[k - 1];
        /* numerator: (2k)(2k-1)*7*(7k-4)*(7k-3) */
        seq_t num = (seq_t)(2 * k) * (seq_t)(2 * k - 1);
        num *= (seq_t)7;
        num *= (seq_t)(7 * k - 4);
        num *= (seq_t)(7 * k - 3);

        /* denominator: k^4 */
        seq_t denom = (seq_t)k * (seq_t)k * (seq_t)k * (seq_t)k;

        /* cancel common factors with cur and denom */
        seq_t g = gcd(cur, denom);
        cur   /= g;
        denom /= g;

        g = gcd(num, denom);
        num   /= g;
        denom /= g;

        /* multiply then divide (denom should be 1 now) */
        cur = cur * num;
        cur /= denom;

        cache[k] = cur;
    }

    computed = n + 1;
    return cache[n];
}

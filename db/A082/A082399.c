/*
 * A082399  a(1) = 1; thereafter, a(n) is the smallest nonnegative number such
 *          that the number Sum_{i=1..n} a(i)*10^(n-i) is divisible by n.
 * https://oeis.org/A082399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest nonnegative integer such that Sum_{i=1..n} a(i)
 *            * 10^(n-i) is divisible by n
 * Range:     u8 n <= 277, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;        /* undefined below the offset */

    /* 1‑based cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;        /* highest index that has been computed */
    static size_t cache_cap = 0;       /* allocated size of the cache array */

    /* grow the cache if necessary; we keep at least n+1 slots
       (index 0 is unused) */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1)) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = (seq_t)1;
        } else {
            idx_t rem = 0;
            for (idx_t i = 1; i < k; ++i) {
                rem = (rem * 10 + (idx_t)cache[i]) % k;
            }
            idx_t t = (rem * 10) % k;
            cache[k] = (seq_t)((k - t) % k);
        }
    }

    cache_len = n;        /* now we have computed up to n */
    return cache[n];
}

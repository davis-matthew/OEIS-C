/*
 * A130044  a(0)=1. a(n) = number of earlier terms which are coprime to the
 *          largest odd divisor of n.
 * https://oeis.org/A130044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements sequence A130044:
 *   a(0) = 1
 *   For n > 0: a(n) = number of indices i < n such that gcd(a(i),
 *   L(n)) = 1, where L(n) is the largest odd divisor of n.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache is allocated and contains a(0) */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0; /* allocation failure */
        cache[0] = (seq_t)1;
    }

    /* Grow cache to cover index n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return 0;        /* allocation failure */
        cache = tmp;

        for (idx_t k = cache_len; k <= n; ++k) {
            if (k == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            /* largest odd divisor of k */
            idx_t m = k;
            while ((m & 1) == 0) m >>= 1;
            seq_t d = (seq_t)m;

            /* Count earlier terms coprime with d */
            seq_t cnt = (seq_t)0;
            for (idx_t i = 0; i < k; ++i) {
                if (gcd(cache[i], d) == (seq_t)1) ++cnt;
            }
            cache[k] = cnt;
        }
        cache_len = new_len;
    }

    return cache[n];
}

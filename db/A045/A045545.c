/*
 * A045545  a(0) = 1; a(n) = Sum_{0 <= k < n and gcd(k,n) = 1} a(k).
 * https://oeis.org/A045545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{0 ≤ k < n, gcd(k,n)=1} a(k) a(0) = 1 a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 40, u64 n <= 78, u128 n <= 154,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache for previously computed a(i) */
    static size_t cap = 0;          /* current allocated capacity */
    static idx_t max_calculated = 0;/* highest index that has been computed */

    /* initialise cache for a(0) and a(1) */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        max_calculated = 1;
    }

    /* grow the cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed – return 0 as a safe value */
        cache = new_cache;
        cap = new_cap;
    }

    /* compute any missing values up to n */
    if (n > max_calculated) {
        for (idx_t m = max_calculated + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 0; k < m; ++k) {
                if (gcd((seq_t)k, (seq_t)m) == 1) {
                    sum += cache[k];
                }
            }
            cache[m] = sum;
        }
        max_calculated = n;
    }

    return cache[n];
}

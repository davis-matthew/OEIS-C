/*
 * A080823  a(n)=n for n<=4; for n>4, a(n) is the smallest integer greater
 *          than a(n-1) that shares common factors with some (i.e., at least
 *          one) but not all of previous 4 terms.
 * https://oeis.org/A080823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n for n<=4; for n>4 the smallest integer > a(n-1) that
 *            shares a common factor with at least one of the previous 4 terms
 *            but not with all four. (OEIS A080823)
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Memoised cache of computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (n == 0)           /* undefined for n=0, return 0 */
        return 0;

    /* Return cached result if already computed */
    if (n <= cache_len)
        return cache[n-1];

    /* Expand cache if necessary */
    if (n > cache_cap) {
        size_t new_cap = (size_t)n * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i-1] = (seq_t)i;
            continue;
        }

        seq_t candidate = cache[i-2] + (seq_t)1;   /* start just above a(i-1) */
        while (true) {
            bool has_common = false;
            bool has_coprime = false;
            for (int k = 0; k < 4; ++k) {
                seq_t g = gcd(candidate, cache[i-2-k]);
                if (g > (seq_t)1) has_common = true;
                if (g == (seq_t)1) has_coprime = true;
                if (has_common && has_coprime) break;
            }
            if (has_common && has_coprime)
                break;          /* candidate satisfies the condition */
            candidate += (seq_t)1;
        }
        cache[i-1] = candidate;
    }

    cache_len = n;
    return cache[n-1];
}

/*
 * A139558  a(1)=4. For n>=2, a(n) = smallest integer > a(n-1) that is not
 *          coprime to (n+1) but is coprime to n.
 * https://oeis.org/A139558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1235, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139558
 * a(1) = 4
 * For n >= 2: a(n) = smallest integer > a(n-1) that is not coprime to (n+1) but is coprime to n.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap = 0;
    if (n == 0) return 0;            /* offset is 1; not used in tests */

    /* Ensure at least one entry for a(1) */
    if (n == 1) {
        if (cache_size < 1) {
            if (!cache) {
                cache_cap = 4;
                cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
                if (!cache) exit(1);
            }
            cache[0] = 4;
            cache_size = 1;
        }
        return cache[0];
    }

    /* Expand the cache if a(n) has not been computed yet */
    if (n > cache_size) {
        if (n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            if (new_cap < n) new_cap = n;
            seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!new_cache) exit(1);
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* Compute missing terms up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];      /* a(i-1) */
            seq_t candidate = prev + 1;
            seq_t cur = (seq_t)i;           /* n */
            seq_t curp1 = cur + 1;          /* n+1 */

            while (true) {
                if (gcd(candidate, cur) == 1 && gcd(candidate, curp1) != 1)
                    break;
                ++candidate;
            }
            cache[i - 1] = candidate;       /* store a(i) */
        }
        cache_size = n;
    }

    return cache[n - 1];
}

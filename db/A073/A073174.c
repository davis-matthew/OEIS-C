/*
 * A073174  a(n) = a(phi(n-1)) + a(phi(n-2)), a(0)=0, a(1)=1.
 * https://oeis.org/A073174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(phi(n-1)) + a(phi(n-2)); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 149, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0; /* largest index that has a valid value */

    /* Ensure capacity for indices up to n (inclusive) */
    size_t needed = (size_t)n + 1;
    if (needed < 2)
        needed = 2;                     /* at least hold a(0) and a(1) */

    if (needed > (size_t)cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_size = (idx_t)needed;
    }

    /* Initialise base cases once */
    if (max_computed < 1) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* If already computed, return */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing entries up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t phi1 = phi(i - 1);
        seq_t phi2 = phi(i - 2);
        idx_t p1 = (idx_t)phi1;
        idx_t p2 = (idx_t)phi2;
        cache[i] = cache[p1] + cache[p2];
    }
    max_computed = n;
    return cache[n];
}

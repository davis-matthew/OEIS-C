/*
 * A052691  Expansion of e.g.f. (1-x)/(1-2*x+x^2-x^3).
 * https://oeis.org/A052691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 2 a(n) = 2*n*a(n-1) - n*(n-1)*a(n-2)
 *            + n*(n-1)*(n-2)*a(n-3) (OEIS A052691, expansion of
 *            (1-x)/(1-2*x+x^2-x^3)) This implementation memoises previously
 *            computed terms in a dynamic array so each call runs in O(max(n))
 *            time in total
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t max_computed = 0;   /* highest n computed, inclusive */

    /* Ensure array large enough for n */
    size_t idx = (size_t)n;
    if (idx + 1 > cache_size) {
        size_t new_size = cache_size ? cache_size : 8;
        while (new_size <= idx) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – unlikely in tests */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise base cases once */
    if (max_computed < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        max_computed = 2;
    }

    /* Extend cache up to requested n using the recurrence */
    if (idx > max_computed) {
        for (size_t i = max_computed + 1; i <= idx; ++i) {
            seq_t nst   = (seq_t)i;
            seq_t term  = 2 * nst * cache[i-1];
            term -= nst * (seq_t)(i-1) * cache[i-2];
            term += nst * (seq_t)(i-1) * (seq_t)(i-2) * cache[i-3];
            cache[i] = term;
        }
        max_computed = idx;
    }

    return cache[idx];
}

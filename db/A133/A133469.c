/*
 * A133469  a(n) = a(n-1) + 7*a(n-2) for n >= 2, a(0)=1, a(1)=2.
 * https://oeis.org/A133469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 7*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity in elements */
    static idx_t max_cached = 0;    /* largest index computed so far */

    /* handle base cases without dynamic allocation */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* first call: allocate minimal cache */
    if (cache == NULL) {
        cap = 2;
        cache = seq_alloc_array(cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        max_cached = 1;
    }

    /* if requested index beyond current capacity, grow array */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap <<= 1;
        seq_t *tmp = seq_alloc_array(newcap);
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
        seq_free_array(cache);
        cache = tmp;
        cap = newcap;
    }

    /* compute missing values up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[(size_t)i] = cache[(size_t)(i - 1)] + (seq_t)7 * cache[(size_t)(i - 2)];
    }
    max_cached = n;

    return cache[(size_t)n];
}

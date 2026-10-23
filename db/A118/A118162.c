/*
 * A118162  Start with 1 and repeatedly reverse the digits and add 60 to get
 *          the next term.
 * https://oeis.org/A118162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n ≥ 2, a(n) = reverse_digits(a(n‑1)) + 60 */

#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    /* cache[0] holds a(1), cache[1] holds a(2), ... */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values currently cached */
    static size_t cache_cap = 0;   /* allocated capacity of cache */

    if (n == 0)
        return (seq_t)0;          /* undefined below offset */

    /* ensure cache is allocated */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first term if not present */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;  /* a(1) */
        cache_len = 1;
    }

    /* generate missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t rev = reverse_digits(cache[i - 1]);
        cache[i] = rev + (seq_t)60;
    }
    cache_len = (size_t)n;

    return cache[n - 1];   /* convert 1-based index to 0-based array */
}

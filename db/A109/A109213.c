/*
 * A109213  Product of a(n-2) and digit reversal of a(n-1).
 * https://oeis.org/A109213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-2)*rev(a(n-1)), with * a(1) = 1, a(2) = 2. rev(x) is
 *            the decimal digit reversal
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/rev.h"

/* Cache for terms, 1-based indexing */
static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t sz = 0;

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cap) return;
    size_t new_cap = cap ? cap : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset 1: undefined for n==0 */

    ensure_cache(n);

    /* initialise the first two terms if not yet done */
    if (sz < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        sz = 2;
    }

    if (n <= sz) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = sz + 1; i <= n; ++i) {
        cache[i] = cache[i-2] * rev(cache[i-1]);
    }

    sz = n;
    return cache[n];
}

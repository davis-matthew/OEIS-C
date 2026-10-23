/*
 * A123222  Expansion of -x * (x-1) * (3*x^2-1) / (9*x^4-8*x^3+4*x-1).
 * https://oeis.org/A123222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) + 9*a(n-4) (a(1)=1, a(2)=3, a(3)=9,
 *            a(4)=31)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* if n < 1, sequence is undefined; return 0 as a fallback */
    if (n < 1) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  size  = 0; /* highest index computed so far */

    /* initialize cache with initial terms */
    if (!cache) {
        cap  = 16;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();          /* cannot allocate */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)31;
        size   = 4;
    }

    /* grow cache to accommodate index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* cannot allocate */
        cache = tmp;
        /* newly allocated area is zero‑initialized by realloc semantics? */
        cap = new_cap;
    }

    /* compute terms up to n if needed */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t v1 = (seq_t)4 * cache[i - 1];
        seq_t v2 = (seq_t)8 * cache[i - 3];
        seq_t v3 = (seq_t)9 * cache[i - 4];
        cache[i] = v1 - v2 + v3;
    }
    if (n > size) size = n;

    return cache[n];
}

/*
 * A135414  a(1)=a(2)=1 and for n>=3, a(n) = n - a(a(n-2)).
 * https://oeis.org/A135414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1, a(n)=n - a(a(n-2)) for n ≥ 3
 * Range:     u8 n <= 413, u16 n <= 1999, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), ... */
    static size_t cap   = 0;       /* allocated capacity of cache          */
    static size_t len   = 0;       /* number of entries currently stored   */

    if (n < 1) return 0;           /* undefined for 0, but return 0 */

    /* Initialise cache on first call */
    if (!cache) {
        cap = 4;                   /* small initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */
        cache[0] = 1;              /* a(1) */
        cache[1] = 1;              /* a(2) */
        len = 2;
    }

    /* Expand cache if needed */
    if ((idx_t)len < n) {
        size_t newcap = cap;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed */
        cache = tmp;
        cap   = newcap;

        /* Compute missing terms in sequence order */
        for (size_t i = len; i < (size_t)n; ++i) {
            idx_t idx = (idx_t)i + 1;                  /* 1‑based index */
            idx_t prev2 = idx - 2;                      /* (n-2) */
            seq_t prev2_val = cache[(size_t)prev2 - 1]; /* a(n‑2) */
            seq_t inner = cache[(size_t)prev2_val - 1]; /* a(a(n‑2)) */
            cache[i] = (seq_t)idx - inner;              /* a(n) = n - a(a(n-2)) */
        }
        len = n;
    }

    return cache[n - 1];  /* return a(n) */
}

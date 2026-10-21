/*
 * A077382  a(1) = 1, then multiply, add and subtract 2, 3, 4; 5, 6, 7; ... in
 *          that order.
 * https://oeis.org/A077382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077382
   a(1) = 1;
   For n > 1:
       if n mod 3 == 2  --> a(n) = a(n-1) * n
       if n mod 3 == 0  --> a(n) = a(n-1) + n
       if n mod 3 == 1  --> a(n) = a(n-1) - n
*/

#include <stdlib.h>

static seq_t *seq_cache = NULL;  /* cache[1] is a(1) */
static idx_t cache_len = 0;       /* number of terms stored in cache (>=0) */

seq_t a(idx_t n) {
    /* handle undefined n < 1 */
    if (n < 1) return (seq_t)0;

    /* one‑time cache initialization */
    if (!seq_cache) {
        cache_len = 1;
        seq_cache = (seq_t *)malloc(2 * sizeof(seq_t)); /* indices 0 and 1 */
        if (!seq_cache) return (seq_t)0;                /* allocation failed */
        seq_cache[0] = (seq_t)0;                        /* unused */
        seq_cache[1] = (seq_t)1;                        /* a(1) = 1 */
    }

    /* return cached value if already computed */
    if (n <= cache_len) return seq_cache[n];

    /* grow cache to size n+1 (indices 0..n) */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;           /* allocation failed */
    seq_cache = tmp;

    /* compute missing terms, starting after the largest stored index */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = seq_cache[i - 1];
        idx_t mod = i % 3;
        if (mod == 2) {                     /* multiply */
            seq_cache[i] = prev * (seq_t)i;
        } else if (mod == 0) {               /* add */
            seq_cache[i] = prev + (seq_t)i;
        } else {                              /* subtract */
            seq_cache[i] = prev - (seq_t)i;
        }
    }

    cache_len = n;
    return seq_cache[n];
}

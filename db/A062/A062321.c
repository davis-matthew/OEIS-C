/*
 * A062321  a(1) = 1; a(n) = gcd {n*(n+1), a(n-1) * (a(n-1) + 1)} for n > 1.
 * https://oeis.org/A062321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1,
   a(n)=gcd( n*(n+1), a(n-1)*(a(n-1)+1) ).
   The function uses memoisation so that calls can be made in any order. */

#include <stdlib.h>
#include "../utils/seq_gcd_local.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* safety – offset in OEIS starts at 1 */

    /* Static cache: cache[i] holds a(i) for i >= 1 */
    static seq_t *cache = NULL;      /* 1‑based array, cache[0] unused */
    static idx_t cached_n = 0;       /* highest index computed so far */

    /* Allocate or grow cache if necessary */
    if (cache == NULL) {
        /* first allocation, just enough for index n (1‑based) */
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[1] = (seq_t)1;   /* a(1)=1 */
        cached_n = 1;
    } else if (n > cached_n) {
        /* extend cache to cover up to n */
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) return cache[cached_n];   /* keep old data */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t term1 = (seq_t)i * (((seq_t)i) + (seq_t)1);
            seq_t term2 = prev * (prev + (seq_t)1);
            cache[i] = seq_gcd_local(term1, term2);
        }
        cached_n = n;
    }
    return cache[n];
}

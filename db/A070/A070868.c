/*
 * A070868  a(1) = a(2) = 1; a(n) = a(a(n-1)) + a(n+1 - 2*a(n-1)).
 * https://oeis.org/A070868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 1; a(n) = a(a(n-1)) + a(n + 1 - 2*a(n-1)) (n ≥
 *            3)
 * Range:     u8 n <= 2537, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values, 1‑based */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* allocated indices [0 .. capacity-1] */
    static idx_t last = 0;       /* largest n computed so far */

    if (n == 1 || n == 2)
        return (seq_t)1;

    /* grow the cache if necessary */
    if (n >= capacity) {
        idx_t new_cap = n + 1;                /* store up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort with 0 (should not happen) */
            free(cache);
            cache = NULL;
            capacity = 0;
            last = 0;
            return (seq_t)0;
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* initialise the first two terms if this is the first run */
    if (last < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        last = 2;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i-1) is already known */
        idx_t prev = (idx_t)cache[i - 1];
        /* second index: i + 1 - 2*prev  (guaranteed ≥ 1 and < i) */
        idx_t second = i + 1 - 2 * prev;
        cache[i] = cache[prev] + cache[second];
    }
    last = n;

    return cache[n];
}

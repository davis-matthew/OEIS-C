/*
 * A230393  Number of n X 2 0..4 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 5, and upper left element zero.
 * https://oeis.org/A230393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-3) - 9*a(n-4) - 6*a(n-5) (n ≥ 6) Initial
 *            values: a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=2
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoized values */
    static seq_t *cache   = NULL;   /* pointer to cache array (1‑based) */
    static idx_t max_computed = 0;   /* highest index that has been computed */
    static idx_t capacity     = 0;   /* allocated size of cache array */

    /* ensure cache has room for indices up to n */
    if (capacity <= n) {
        idx_t new_cap = (capacity == 0) ? (n + 1) : (capacity * 2);
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – return 0 as fallback */
        cache   = tmp;
        capacity = new_cap;
    }

    /* initialise base values (if not already done) */
    if (max_computed < 5) {
        /* ensure indices 1..5 exist */
        if (capacity < 6) {
            seq_t *tmp = (seq_t*)realloc(cache, 6 * sizeof(seq_t));
            if (!tmp) return 0;
            cache   = tmp;
            capacity = 6;
        }
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
        cache[5] = 2;
        max_computed = 5;
    }

    /* compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t = 4 * cache[i-1];
        t -= 4 * cache[i-3];
        t -= 9 * cache[i-4];
        t -= 6 * cache[i-5];
        cache[i] = t;
    }
    max_computed = n;
    return cache[n];
}

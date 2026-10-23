/*
 * A037709  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2,0.
 * https://oeis.org/A037709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value in base 4 of the first n digits of the repeating
 *            pattern 1,3,2,0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is equivalent to a(1)=1, a(n)=a(n-1)*4 + pattern[(n-1)%4]. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms: cache[i] = a(i+1) */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity in terms of number of terms */
    static idx_t cache_len = 0;   /* number of terms already computed */

    if (n == 0) return 0; /* undefined, return 0 for safety */

    /* Expand cache if necessary */
    if (n > (idx_t)cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) {
            /* allocation failed, abort (no error handling required by spec) */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    static const seq_t pattern[4] = { (seq_t)1, (seq_t)3, (seq_t)2, (seq_t)0 };
    for (idx_t i = cache_len; i < n; ++i) {
        if (i == 0) {
            cache[i] = pattern[0];
        } else {
            cache[i] = cache[i - 1] * (seq_t)4 + pattern[(i) % 4];
        }
    }
    cache_len = n;

    return cache[n - 1];
}

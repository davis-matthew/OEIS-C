/*
 * A171857  Number of n-step up-side self-avoiding walks on the lattice strip
 *          {0,1,2} x Z (up-side means that the walks move up and sideways but
 *          not down).
 * https://oeis.org/A171857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) + a(n-4), * a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=8 * This recurrence follows from the generating
 *            function * (1 + z^2 + z^3)/(1 - 2z + z^2 - z^3 - z^4). The
 *            sequence * is memoised in a static array so that the first call
 *            * computing any value up to n performs O(n) work an...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of results */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = 3;  /* indices 0..3 are base cases */

    /* initialise base cases on first call */
    if (cache == NULL) {
        cache_size = 8;  /* arbitrary small initial size */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        /* zero initialise rest of allocated space */
        for (size_t i = 4; i < cache_size; ++i) cache[i] = 0;
    }

    /* enlarge the cache if needed */
    if ((idx_t)(n + 1) > (idx_t)cache_size) {
        size_t new_size = (size_t)n + 1;
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        /* zero initialise newly allocated part */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* compute values up to n if not yet computed */
    if ((idx_t)(last_computed) < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-1) - a(i-2) + a(i-3) + a(i-4) */
            seq_t term = 2 * cache[i - 1];
            term = term - cache[i - 2];
            term = term + cache[i - 3];
            term = term + cache[i - 4];
            cache[i] = term;
        }
        last_computed = n;
    }

    return cache[n];
}

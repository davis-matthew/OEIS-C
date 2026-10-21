/*
 * A050400  Number of independent sets of vertices in P_3 X C_n (n > 2).
 * https://oeis.org/A050400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-2) + 6*a(n-3) - a(n-4) - a(n-5) with
 *            initial values a(0)=5, a(1)=1, a(2)=17, a(3)=43, a(4)=181. The
 *            function evaluates the recurrence iteratively, memoising
 *            previously computed terms
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t base[5] = {5, 1, 17, 43, 181};

    /* Cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* For the first five indices, return the hard‑coded values */
    if (n < 5)
        return base[n];

    /* Initialise cache with the first five values on first use */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = base[i];
    }

    /* Extend the cache up to n if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;        /* n is idx_t (uint64_t) */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) /* allocation failed – abort with 0 */
            return 0;
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = cache[i-1]
                      + (seq_t)8   * cache[i-2]
                      + (seq_t)6   * cache[i-3]
                      -            cache[i-4]
                      -            cache[i-5];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}

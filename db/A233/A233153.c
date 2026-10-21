/*
 * A233153  Number of n X 6 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally or antidiagonally.
 * https://oeis.org/A233153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) - 90*a(n-2) + 81*a(n-3) a(1) = 96, a(2) = 1944,
 *            a(3) = 40392
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0) return 0;  /* undefined for offset 1, return 0 */

    /* Grow cache if needed */
    if (cache_len < (size_t)n) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort or handle error */
            abort();
        }
        cache = tmp;
        /* Compute new entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            idx_t k = (idx_t)(i + 1); /* actual index, 1‑based */
            if (k == 1)
                cache[i] = (seq_t)96;
            else if (k == 2)
                cache[i] = (seq_t)1944;
            else if (k == 3)
                cache[i] = (seq_t)40392;
            else {
                seq_t val = (seq_t)25 * cache[i-1];
                val -= (seq_t)90 * cache[i-2];
                val += (seq_t)81 * cache[i-3];
                cache[i] = val;
            }
        }
        cache_len = new_len;
    }

    return cache[n-1];
}

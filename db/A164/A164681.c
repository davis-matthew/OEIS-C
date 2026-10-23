/*
 * A164681  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 39 a(n) = 38*a(n-1) for 2 ≤ n ≤ 6 a(7) = 38*a(6)
 *            - 740*a(0) - 1 a(8) = 38*a(7) - 740*a(1) + 703*a(0) - 1 a(n) =
 *            38*a(n-1) - 740*a(n-7) + 703*a(n-8) for n ≥ 9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* number of entries currently stored */

    /* Grow the cache if necessary */
    if (n >= cache_size) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – unlikely */
        cache = tmp;

        /* Compute missing values consecutively */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            }
            else if (i == 1) {
                val = (seq_t)39;
            }
            else if (i <= 6) {
                val = (seq_t)38 * cache[i - 1];
            }
            else if (i == 7) {
                val = (seq_t)38 * cache[6] - (seq_t)740 * cache[0] - (seq_t)1;
            }
            else if (i == 8) {
                val = (seq_t)38 * cache[7]
                      - (seq_t)740 * cache[1]
                      + (seq_t)703 * cache[0]
                      - (seq_t)1;
            }
            else {
                val = (seq_t)38 * cache[i - 1]
                      - (seq_t)740 * cache[i - 7]
                      + (seq_t)703 * cache[i - 8];
            }
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}

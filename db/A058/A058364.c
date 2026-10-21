/*
 * A058364  Number of ways to cover (without overlapping) a ring lattice
 *          (necklace) of n sites with molecules that are 9 sites wide.
 * https://oeis.org/A058364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) for n ≥ 10 a(n) = 1 for 1 ≤ n ≤ 8, a(9) =
 *            10 (Offset 1)
 * Range:     u8 n <= 28, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* undefined index 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* static cache: a(1)..a(max_cached) stored at cache[0]..cache[max_cached-1] */
    static seq_t *cache        = NULL;
    static idx_t cache_size    = 0;
    static idx_t max_cached    = 0;

    /* enlarge cache if necessary */
    if (n > cache_size) {
        seq_t *newc = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (newc == NULL) {
            /* allocation failure – unlikely for given limits */
            return (seq_t)0;
        }
        cache      = newc;
        cache_size = n;
    }

    /* compute values that are not yet cached */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i - 1] = (seq_t)1;
        } else if (i == 9) {
            cache[i - 1] = (seq_t)10;
        } else {
            /* a(i) = a(i-1) + a(i-9) */
            cache[i - 1] = cache[i - 2] + cache[i - 10];
        }
    }
    max_cached = n;

    return cache[n - 1];
}

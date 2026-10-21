/*
 * A220548  Number of ways to reciprocally link elements of an n X 3 array
 *          either to themselves or to exactly one horizontal, vertical or
 *          antidiagonal neighbor.
 * https://oeis.org/A220548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 17*a(n-2) - 3*a(n-3) - 17*a(n-4) + 3*a(n-5) +
 *            4*a(n-6) - a(n-7); Initial values: a(1)=3, a(2)=29, a(3)=224,
 *            a(4)=1814, a(5)=14556, a(6)=117022, a(7)=940432
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* Values are defined for n>=1 */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t   cap   = 0;           /* Capacity in terms */
    static idx_t   len   = 0;           /* Number of stored terms */

    /* Initial values up to a(7) */
    static seq_t init_vals[7] = {
        (seq_t)3, (seq_t)29, (seq_t)224,
        (seq_t)1814, (seq_t)14556,
        (seq_t)117022, (seq_t)940432
    };

    /* Grow the cache if necessary */
    if (cap < n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) return 0;           /* Allocation failed – return 0 */
        cache = tmp;
        cap   = n;
    }

    /* Fill in the initial values if we haven't done so yet */
    if (len < n) {
        if (len < 7) {
            idx_t upto = (n < 7 ? n : 7);
            for (idx_t i = len; i < upto; ++i)
                cache[i] = init_vals[i];
            len = upto;
        }

        /* Compute remaining terms using the recurrence
           (valid for i >= 7 i.e. for n >= 8). */
        for (idx_t i = len; i < n; ++i) {
            cache[i] = 6 * cache[i-1]
                     + 17 * cache[i-2]
                     - 3  * cache[i-3]
                     - 17 * cache[i-4]
                     + 3  * cache[i-5]
                     + 4  * cache[i-6]
                     -    cache[i-7];
        }
        len = n;
    }

    return cache[n-1];
}

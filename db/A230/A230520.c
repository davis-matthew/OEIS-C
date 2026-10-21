/*
 * A230520  Number of n X 2 0..2 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n‑1) + 9*a(n‑2) + 18*a(n‑3) (n ≥ 4), with initial
 *            values a(1) = 0, a(2) = 12, a(3) = 78
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[i] == a(i+1) for i ≥ 0 */
    static idx_t cache_len = 0;   /* number of terms currently cached */

    if (n == 0) return 0;          /* not used, harmless */

    /* If we already have a(n), return it */
    if (n <= cache_len) return cache[n-1];

    /* Remember the length before we reallocate */
    idx_t old_len = cache_len;

    /* Grow the cache to hold up to a(n) */
    cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!cache) return 0;          /* allocation failure */

    cache_len = n;

    /* Ensure the initial terms are set, but not beyond the new size */
    idx_t max_init = (n < 3 ? n : 3);
    if (max_init >= 1) cache[0] = (seq_t)0;          /* a(1) */
    if (max_init >= 2) cache[1] = (seq_t)12;         /* a(2) */
    if (max_init >= 3) cache[2] = (seq_t)78;          /* a(3) */

    /* Compute any new terms using the recurrence */
    idx_t start = old_len;
    if (start < 3) start = 3; /* start from the first term that
                                requires the recurrence */
    for (idx_t i = start; i < n; ++i) {
        cache[i] = (seq_t)6  * cache[i-1] +
                   (seq_t)9  * cache[i-2] +
                   (seq_t)18 * cache[i-3];
    }

    return cache[n-1];
}

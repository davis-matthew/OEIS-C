/*
 * A204708  Number of (n+1) X 4 0..1 arrays with the permanents of all 2 X 2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 4*a(n-3) ; initial values: a(1)=25,
 *            a(2)=81, a(3)=257
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Common iterative memoization suitable for any build */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* current cache size (maximum n+1) */

    if (n < 1)
        return 0;                 /* undefined for n < 1, but safe return */

    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;     /* we need indices 0..n */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;

        /* initialize newly needed entries */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i == 0)
                continue;          /* unused, keep 0 */
            if (i == 1)
                cache[i] = (seq_t)25;
            else if (i == 2)
                cache[i] = (seq_t)81;
            else if (i == 3)
                cache[i] = (seq_t)257;
            else {
                seq_t t1 = (seq_t)3 * cache[i-1];
                seq_t t2 = (seq_t)2 * cache[i-2];
                seq_t t3 = (seq_t)4 * cache[i-3];
                cache[i] = t1 + t2 - t3;
            }
        }
    }
    return cache[n];
}

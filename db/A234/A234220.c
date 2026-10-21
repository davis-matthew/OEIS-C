/*
 * A234220  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 20*a(n-2) - 80*a(n-3); Initial terms:
 *            a(1)=104, a(2)=436, a(3)=1824. Offset: 1 (a(1) is the first
 *            term)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle out-of-range or zero index gracefully */
    if (n == 0)
        return 0;

    /* Static cache to store previously computed values */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;

    /* Expand cache if necessary */
    if (n > cached_max) {
        size_t new_size = (size_t)(n + 1);          /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) /* allocation failed: terminate */
            abort();
        cache = tmp;

        /* Fill in new entries */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = 104;
            } else if (i == 2) {
                cache[2] = 436;
            } else if (i == 3) {
                cache[3] = 1824;
            } else {
                /* Recurrence: a(i) = 4*a(i-1) + 20*a(i-2) - 80*a(i-3) */
                const seq_t four   = 4;
                const seq_t twenty = 20;
                const seq_t eighty = 80;
                cache[i] = four  * cache[i - 1] +
                           twenty * cache[i - 2] -
                           eighty * cache[i - 3];
            }
        }
        cached_max = n;
    }

    return cache[n];
}

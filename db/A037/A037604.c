/*
 * A037604  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3.
 * https://oeis.org/A037604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-3) - 4*a(n-4) for n > 5, a(1) = 1, a(2) =
 *            6, a(3) = 27, a(4) = 109, a(5) = 438
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    static seq_t *cache   = NULL;   /* cache[i] holds a(i+1) */
    static idx_t cache_len = 0;
    static const seq_t init[5] = {1, 6, 27, 109, 438};

    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */

        /* copy existing cache */
        for (idx_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 5) {
                tmp[i - 1] = init[i - 1];
            } else {
                tmp[i - 1] = (seq_t)4 * tmp[i - 2] + tmp[i - 4] - (seq_t)4 * tmp[i - 5];
            }
        }

        free(cache);
        cache = tmp;
        cache_len = new_len;
    }

    return cache[n - 1];
}

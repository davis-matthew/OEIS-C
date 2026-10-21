/*
 * A093349  A 7-fractal "castle" starting with 0.
 * https://oeis.org/A093349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 343, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power_of_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A093349: 1-fractal "castle" starting with 0
   a(1) = 0
   a(n) = w(n) - a(n - w(n)),  n > 1
   where w(n) = 7 ^ ⌊log_7(n-1)⌋
   (equivalently a(n) = Σ_{i=1}^{n-1} (-1)^{i-1} 7^{ν_7(i)}	*/
#include <stdlib.h>
#include <stdint.h>
#include "../utils/power_of_7.h"

static seq_t *cache = NULL;      /* memoised values a(1), a(2), …           */
static idx_t cache_size = 0;     /* number of values currently stored      */

seq_t a(idx_t n)
{
    if (n < 1)                     /* offset is 1; undefined for n < 1 */
        return (seq_t)0;

    /* Grow the cache if necessary */
    if (n > cache_size)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache)           /* allocation failed – return 0 */
            return (seq_t)0;
        cache = new_cache;

        /* Compute each missing term only once */
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            if (i == 1)
            {
                cache[0] = (seq_t)0;
                continue;
            }
            seq_t w_seq = power_of_7(i);          /* w(i) as seq_t  */
            idx_t w_idx = (idx_t)w_seq;           /* convert to idx_t  */
            idx_t pos_index = (i - w_idx) - 1;    /* zero‑based index of a(i-w) */
            cache[i - 1] = w_seq - cache[pos_index];
        }
        cache_size = n;
    }

    return cache[n - 1];
}

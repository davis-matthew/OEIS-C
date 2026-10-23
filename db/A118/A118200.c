/*
 * A118200  Start with 1 and repeatedly reverse the digits and add 66 to get
 *          the next term.
 * https://oeis.org/A118200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less, easy
 * Formula:   a(n) = reverse_decimal(a(n-1)) + 66 with a(1) = 1. The sequence
 *            is memoised
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   ensure_cache, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/reverse_decimal.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /*  cache[1] holds a(1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;

    if (n < 1)
        return 0;   /* undefined below offset, but return 0 */

    if (cache == NULL) {
        ensure_cache(1, &cache, &cache_cap);   /* reserve space for index 1 */
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    if (n > cache_max) {
        /* extend cache to reach index n */
        ensure_cache(n, &cache, &cache_cap);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t rev_val = reverse_decimal(cache[i - 1]);
            cache[i] = rev_val + (seq_t)66;
        }
        cache_max = n;
    }

    return cache[n];
}

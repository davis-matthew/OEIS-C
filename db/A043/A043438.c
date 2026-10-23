/*
 * A043438  Numbers having two 4's in base 8.
 * https://oeis.org/A043438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑8 representation contains exactly two digits
 *            equal to 4
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   count_4_oct, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/count_4_oct.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t current_val = 0;

    if (n == 0)
        return 0;               /* not used, offset is 1 */

    if (n <= cache_len)
        return cache[n - 1];

    /* ensure the cache can hold at least n terms */
    ensure_cache((idx_t)n, &cache, &cache_cap);

    for (seq_t x = current_val + 1;; ++x) {
        if (count_4_oct(x) == 2) {
            cache[cache_len++] = x;
            current_val = x;
            if (cache_len == n)
                return x;
        }
    }
}

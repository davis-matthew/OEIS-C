/*
 * A199770  Self-convolution with "addition" played by bitwise XOR.
 * https://oeis.org/A199770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 958 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Recurrence: a(1)=1; for n>=2, a(n)=∑_{i=1}^{n-1} (a(i) XOR a(n-i)) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)
        return (seq_t)0;   /* undefined for offset, but harmless */

    if (n > cache_size) {
        seq_t *new_cache;
        if (cache == NULL)
            new_cache = mem_new(seq_t, (size_t)(n + 1));
        else
            new_cache = mem_resize(seq_t, cache, (size_t)(n + 1));
        if (new_cache == NULL)
            return (seq_t)0;   /* allocation failure */

        cache = new_cache;

        /* initialise the first term if this is the first call */
        if (cache_size == 0) {
            cache[1] = (seq_t)1;
            cache_size = 1;
        }

        /* compute all missing terms up to n */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i < k; ++i)
                sum += cache[i] ^ cache[k - i];
            cache[k] = sum;
        }
        cache_size = n;
    }

    return cache[n];
}

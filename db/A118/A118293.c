/*
 * A118293  Start with 18 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A118293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = rev_decimal(a(n-1)) + 1, with a(1) = 18
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n) {
    /* Static cache to store already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0; /* allocated capacity */
    static size_t cache_len = 0; /* number of terms computed */

    /* Allocate or grow cache if needed */
    if (n > cache_cap) {
        /* Ensure capacity for n terms */
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return 0; /* fallback: memory allocation failure */
        cache = new_cache;
        cache_cap = n;
    }

    /* Compute missing terms sequentially */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)18;
        } else {
            seq_t rev = reverse_decimal(cache[i - 1]);
            cache[i] = rev + (seq_t)1;
        }
    }
    cache_len = (size_t)n;

    return cache[(size_t)n - 1];
}

/*
 * A037497  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2.
 * https://oeis.org/A037497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-3) - 4*a(n-4) * initial values: a(1) = 1,
 *            a(2) = 4, a(3) = 18, a(4) = 73
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Expand cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i == 0)          cache[i] = 1;
        else if (i == 1)     cache[i] = 4;
        else if (i == 2)     cache[i] = 18;
        else if (i == 3)     cache[i] = 73;
        else
            cache[i] = ((seq_t)4)*cache[i-1] + cache[i-3] - ((seq_t)4)*cache[i-4];
    }
    cache_len = n;

    return cache[n-1];
}

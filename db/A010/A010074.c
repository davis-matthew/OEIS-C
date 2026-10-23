/*
 * A010074  a(n) = sum of base-7 digits of a(n-1) + sum of base-7 digits of
 *          a(n-2).
 * https://oeis.org/A010074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum_7(a(n-1)) + digit_sum_7(a(n-2)) with a(0)=0,
 *            a(1)=1, a(2)=1 (digit_sum_7(v) = sum of base‑7 digits of v)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, realloc, size_t */
#include "../utils/digit_sum_7.h"

seq_t a(idx_t n)
{
    /* Static cache: index 0 .. cache_len-1 are defined. */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;
    idx_t old_len = cache_len;

    /* Initial terms for n = 0,1,2. */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)realloc(cache, (size_t)cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
    }

    /* Grow cache if needed. */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms from old_len (next free index) up to n. */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i == 0) continue;        /* already set */
        if (i == 1 || i == 2) continue; /* already set */
        seq_t d1 = digit_sum_7(cache[i - 1]);
        seq_t d2 = digit_sum_7(cache[i - 2]);
        cache[i] = d1 + d2;
    }

    return cache[n];
}

/*
 * A122762  a(n) = a(n-2) + a(n-4) + a(n-5) + a(n-7) + a(n-8) + a(n-10) for n
 *          >= 10, with a(0) = ... = a(9) = 1.
 * https://oeis.org/A122762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-4) + a(n-5) + a(n-7) + a(n-8) + a(n-10);
 *            a(0) = … = a(9) = 1. offset 0
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 65, u64 n <= 124,
 *            u128 n <= 243, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – return 0 */
            return 0;
        }
        cache = tmp;

        for (idx_t i = cache_cap; i <= n; ++i) {
            if (i < 10) {
                cache[i] = 1;
            } else {
                cache[i] = cache[i-2] + cache[i-4] + cache[i-5] + cache[i-7]
                           + cache[i-8] + cache[i-10];
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}

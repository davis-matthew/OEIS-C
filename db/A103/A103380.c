/*
 * A103380  a(n) = a(n-12) + a(n-13).
 * https://oeis.org/A103380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-12) + a(n-13), a(1) = a(2) = … = a(13) = 1
 * Range:     u8 n <= 104, u16 n <= 206, u32 n <= 406, u64 n <= 806,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (n < 1). */
    if (n < 1) return 0;

    /* Memoisation cache.  Index 0 is unused; elements 1..current_max hold a(·). */
    static seq_t *cache   = (seq_t *)0;
    static idx_t cur_max  = 0;

    /* Grow the cache if needed.  New size = n + 1 to include index n. */
    if (cur_max < n) {
        idx_t new_size  = n + 1;      /* indices 0 .. n inclusive */
        size_t byte_len = (size_t)new_size * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(
                         (void *) (cache == (seq_t *)0 ? NULL : cache),
                         byte_len);
        if (tmp == (seq_t *)0) {
            /* Allocation failed – fail gracefully. */
            return 0;
        }
        cache = tmp;

        /* Initialise newly allocated slots. */
        for (idx_t i = cur_max + 1; i <= n; ++i) {
            if (i <= 13) {
                cache[i] = (seq_t)1;
            } else {
                cache[i] = cache[i - 12] + cache[i - 13];
            }
        }
        cur_max = n;
    }

    return cache[n];
}

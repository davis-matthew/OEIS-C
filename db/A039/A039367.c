/*
 * A039367  Numbers whose base-9 representation has the same nonzero number of
 *          1's and 4's.
 * https://oeis.org/A039367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   the nth positive integer whose base‑9 representation has * the
 *            same nonzero number of 1's and 4's. The function computes terms
 *            * by a memoised scan of the integers
 * Range:     u8 n <= 18, u16 n <= 8513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Returns the n-th term (n >= 1).  Index 0 is undefined; we return 0. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;   /* number of terms stored   */
    static size_t cache_cap  = 0;   /* current capacity         */

    /* Last number examined during the scan.  Initially just before 13. */
    static seq_t last_checked = 12;

    if (n == 0) return 0;

    /* Grow the cache until we have at least n terms. */
    while (cache_size < (size_t)n) {
        ++last_checked;

        /* Count digits 1 and 4 in base‑9. */
        seq_t temp = last_checked;
        size_t c1 = 0, c4 = 0;
        while (temp > 0) {
            int d = (int)(temp % 9);
            temp /= 9;
            if (d == 1) ++c1;
            else if (d == 4) ++c4;
        }
        if (c1 == c4 && c1 > 0) {
            /* Ensure sufficient capacity. */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                /* In C++, cast is optional; harmless in C. */
                cache = new_cache;
            }
            cache[cache_size++] = last_checked;
        }
    }

    return cache[n - 1];
}

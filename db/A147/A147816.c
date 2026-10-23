/*
 * A147816  Concatenation of n digits 1 and 2(n-1) digits 0.
 * https://oeis.org/A147816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1100*a(n-1) - 100000*a(n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;    /* cache[0] unused, cache[1]..cache[max_n] stored */
    static idx_t max_n = 0;        /* highest index computed and cached */

    if (n < 1) { /* offset is 1, behaviour for n<1 is irrelevant */
        return 0;
    }

    /* Ensure the cache is large enough to hold up to index n */
    if (cache == NULL || n > max_n) {
        idx_t new_size = (n > max_n) ? n : max_n;
        if (cache == NULL) {
            cache = (seq_t *)malloc((new_size + 1) * sizeof(seq_t));
            if (!cache) { /* Allocation failed; return 0 as fallback */
                return 0;
            }
            cache[0] = 0; /* unused */
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
            if (!tmp) { /* Realloc failed; keep old cache */
                return 0;
            }
            cache = tmp;
        }

        /* Compute missing values */
        if (max_n == 0) {
            cache[1] = 1;
            if (n >= 2) cache[2] = 1100;
            max_n = (n >= 2) ? 2 : 1;
        }

        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] = 1100 * cache[i - 1] - 100000 * cache[i - 2];
        }
        max_n = n;
    }

    return cache[n];
}

/*
 * A208790  Number of n X 2 0..2 arrays with no element equal the average of
 *          immediate neighbors vertically above, diagonally above and left,
 *          and horizontally left of it.
 * https://oeis.org/A208790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2) + 10*a(n-3) - 4*a(n-4) (n ≥ 5)
 *            Initial values: a(1)=6, a(2)=30, a(3)=160, a(4)=852
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, return 0 */

    /* Ensure the cache can hold indices up to n */
    if (cache == NULL) {
        /* Allocate space for indices 0..4 (4 needed) */
        cache_size = 5;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;                 /* unused */
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)30;
        cache[3] = (seq_t)160;
        cache[4] = (seq_t)852;
    } else if (cache_size <= (size_t)n) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute new terms up to n if necessary */
    if (n >= 5) {
        for (idx_t i = 5; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i-1]
                     + (seq_t)16 * cache[i-2]
                     + (seq_t)10 * cache[i-3]
                     - (seq_t)4 * cache[i-4];
        }
    }

    return cache[n];
}

/*
 * A140357  a(1)=1; a(n)=floor(4*a(n-1)*(n-a(n-1)) / n) for n > 1.
 * https://oeis.org/A140357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; a(n) = floor(4 * a(n-1) * (n - a(n-1)) / n) for n > 1
 * Range:     u8 n <= 262, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    if (n < 1) return 0;                     /* undefined for n < 1 */

    if (n > cache_max) {
        idx_t new_size = n + 1;               /* allocate up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);       /* allocation failed */
        cache = tmp;

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = 1;
            } else {
                seq_t prev = cache[i - 1];
                /* compute 4*prev*(i-prev) in 64‑bit to avoid overflow */
                uint64_t numerator = 4ULL * (uint64_t)prev * ((uint64_t)i - (uint64_t)prev);
                cache[i] = (seq_t)(numerator / (uint64_t)i);
            }
        }
        cache_max = n;
    }

    return cache[n];
}

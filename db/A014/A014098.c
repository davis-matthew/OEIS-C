/*
 * A014098  a(n) = a(n-1) + a(n-4).
 * https://oeis.org/A014098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4), a(0)=1, a(1)=1, a(2)=1, a(3)=4
 * Range:     u8 n <= 16, u16 n <= 33, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* number of terms currently cached */

    /* Return cached value if possible */
    if (n < cache_size) return cache[n];

    /* Need to extend cache to at least n+1 elements */
    idx_t new_size = n + 1;
    if (!cache) {
        cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failure */
    } else if (new_size > cache_size) {
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failure */
    }

    /* Compute new terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0 || i == 1 || i == 2) {
            cache[i] = 1;
        } else if (i == 3) {
            cache[i] = 4;
        } else {
            cache[i] = cache[i - 1] + cache[i - 4];
        }
    }

    cache_size = new_size;
    return cache[n];
}

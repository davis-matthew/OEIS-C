/*
 * A008895  The 3x-1 map: a(n) = n/2 if n is even, 3n-1 if n is odd.
 * https://oeis.org/A008895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=63, a(n)=a(n‑1)/2 if a(n‑1) even; else 3*a(n‑1)-1 */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Initialise cache with the first term */
    if (cache_len == 0) {
        cache = (seq_t*)realloc(NULL, sizeof(seq_t));
        cache_len = 1;
        cache[0] = 63;
    }

    if (n + 1 > cache_len) {
        /* Grow cache to hold terms up to n */
        size_t new_len = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));

        /* Compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            if ((prev & 1) == 0)          /* even */
                cache[i] = prev >> 1;
            else                          /* odd */
                cache[i] = (seq_t)(
                    ((unsigned __int128)prev) * 3 - 1);
        }
        cache_len = new_len;
    }

    return cache[n];
}

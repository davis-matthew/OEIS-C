/*
 * A165799  Number of tilings of a 4 X n rectangle using right trominoes and 2
 *          X 2 tiles.
 * https://oeis.org/A165799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 9*a(n-3) + a(n-4) * - 3*a(n-5) -
 *            22*a(n-6) - 16*a(n-7) - 4*a(n-9); * Initial values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=4, a(4)=6, * a(5)=16, a(6)=37, a(7)=92,
 *            a(8)=245
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 98,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise cache with known first 9 terms if necessary */
    if (cache == NULL) {
        cache = alloc_seq_array(9);          /* indices 0 .. 8 */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 4;
        cache[4] = 6;
        cache[5] = 16;
        cache[6] = 37;
        cache[7] = 92;
        cache[8] = 245;
        cache_size = 9;
    }

    /* If the requested index is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to accommodate index n */
    idx_t old_size = cache_size;
    seq_t *new_cache = alloc_seq_array((size_t)n + 1);
    for (idx_t i = 0; i < old_size; ++i)
        new_cache[i] = cache[i];
    free_seq_array(cache);
    cache = new_cache;
    cache_size = n + 1;

    /* Compute entries from the first missing index up to n */
    idx_t start = old_size > 9 ? old_size : 9;   /* first index that needs recurrence */
    for (idx_t i = start; i <= n; ++i) {
        seq_t pos = cache[i - 1] + cache[i - 2] + 9 * cache[i - 3] + cache[i - 4];
        seq_t neg = 3 * cache[i - 5] + 22 * cache[i - 6] + 16 * cache[i - 7] + 4 * cache[i - 9];
        cache[i] = pos - neg;   /* Result is guaranteed non‑negative for the sequence */
    }

    return cache[n];
}

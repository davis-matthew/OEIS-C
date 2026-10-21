/*
 * A207769  Number of n X 3 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) - 2*a(n-4) + a(n-6) for n >
 *            7 initial terms (n = 1..7): 6, 36, 84, 192, 450, 972, 2040
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 27, u64 n <= 57, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                               /* offset is 1 */

    static seq_t *cache = NULL;                  /* cache[0] = a(1), etc. */
    static idx_t cache_size = 0;                 /* number of entries allocated */

    /* make sure the cache can hold up to n entries */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp)  /* allocation failed */
            return 0;
        cache = tmp;
        cache_size = n;
    }

    /* initialise base values (they are non‑zero, so overwriting is harmless) */
    if (n >= 1) cache[0] = (seq_t)6;              /* a(1) */
    if (n >= 2) cache[1] = (seq_t)36;             /* a(2) */
    if (n >= 3) cache[2] = (seq_t)84;             /* a(3) */
    if (n >= 4) cache[3] = (seq_t)192;            /* a(4) */
    if (n >= 5) cache[4] = (seq_t)450;            /* a(5) */
    if (n >= 6) cache[5] = (seq_t)972;            /* a(6) */
    if (n >= 7) cache[6] = (seq_t)2040;           /* a(7) */

    if (n <= 7)
        return cache[n-1];

    /* compute more terms using the recurrence */
    for (idx_t i = 8; i <= n; ++i) {              /* i is the current n */
        cache[i-1] = (seq_t)3 * cache[i-2]
                   - (seq_t)2 * cache[i-3]
                   + cache[i-4]
                   - (seq_t)2 * cache[i-5]
                   + cache[i-7];
    }

    return cache[n-1];
}

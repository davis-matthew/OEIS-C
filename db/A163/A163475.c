/*
 * A163475  a(n) = 18*a(n-1) - 78*a(n-2) for n > 1; a(0) = 3, a(1) = 30.
 * https://oeis.org/A163475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 78*a(n-2); a(0) = 3, a(1) = 30
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)30;

    /*  Allocate or grow cache to hold index n */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;                 /* allocation failure fallback */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;                     /* initialize new slots */
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /*  Base values  */
    if (cache[0] == 0) cache[0] = (seq_t)3;
    if (cache[1] == 0) cache[1] = (seq_t)30;

    /*  Return already computed value if available  */
    if (cache[n] != 0)
        return cache[n];

    /*  Compute missing values up to n  */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (cache[i] != 0) continue;          /* already computed in a previous call  */
        seq_t t1 = (seq_t)18 * cache[i - 1];
        seq_t t2 = (seq_t)78 * cache[i - 2];
        cache[i] = t1 - t2;
    }

    return cache[n];
}

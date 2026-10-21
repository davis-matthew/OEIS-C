/*
 * A074475  a(n) = Sum_{j=0..floor(n/2)} T(2*j + q), where T(n) are
 *          generalized tribonacci numbers (A001644) and q = n - 2*floor(n/2).
 * https://oeis.org/A074475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3) + a(n-4) (n ≥ 4) Initial values:
 *            a(0)=3, a(1)=1, a(2)=6, a(3)=8
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 145,
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
    const seq_t init[4] = { (seq_t)3, (seq_t)1, (seq_t)6, (seq_t)8 };

    /* First call: initialise the table to the four known values */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = init[i];
    }

    /* If the requested index is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to hold values up to index n */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (tmp == NULL) return (seq_t)0;              /* allocation failure */
    cache = tmp;
    cache_size = new_size;

    /* Compute missing values using the recurrence */
    for (idx_t k = old_size; k <= n; ++k) {
        /* k ≥ 4 always in this loop because old_size > 4 unless first call */
        seq_t val = 2 * cache[k - 2] + 2 * cache[k - 3] + cache[k - 4];
        cache[k] = val;
    }

    return cache[n];
}

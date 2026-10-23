/*
 * A164769  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n)=6*(a[n-1]+…+a[n-7])−21*a[n-8] for n≥9, with
 *            initial values a(0)=1, a(1)=8, a(2)=56, a(3)=392, a(4)=2744,
 *            a(5)=19208, a(6)=134456, a(7)=941192, a(8)=6588316
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    const seq_t init[9] = {
        (seq_t)1,      (seq_t)8,      (seq_t)56,
        (seq_t)392,    (seq_t)2744,   (seq_t)19208,
        (seq_t)134456, (seq_t)941192, (seq_t)6588316
    };

    /* If we already have the requested value, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate indices up to n. */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – return 0 (this situation shouldn't occur in the tests). */
        return (seq_t)0;
    }
    cache = tmp;
    /* Compute values from the previous size up to the new last index. */
    idx_t old_len = cache_len;
    cache_len = new_len;

    for (idx_t i = old_len; i < new_len; ++i) {
        if (i < 9) {
            /* Use the precomputed initial terms. */
            cache[i] = init[i];
            continue;
        }
        /* Compute a[i] via the recurrence: a[i] = 6*(a[i-1]+…+a[i-7]) - 21*a[i-8]. */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 7; ++k)
            sum += cache[i - k];
        cache[i] = 6 * sum - 21 * cache[i - 8];
    }

    return cache[n];
}

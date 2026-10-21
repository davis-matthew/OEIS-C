/*
 * A084132  a(n) = 4*a(n-1) + 6*a(n-2), a(0)=1, a(1)=2.
 * https://oeis.org/A084132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 6*a(n-2). Initial values: a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of already computed terms   */
    static idx_t cache_cap = 0;          /* allocated size of cache           */
    static idx_t cache_used = 0;         /* number of terms computed so far   */

    /* Ensure we have space for indices 0..n.  Double the capacity as needed. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 2 : cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two terms if they are not yet present. */
    if (cache_used == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_used = 2;
    }

    /* If the requested term is already computed, return it. */
    if (n < cache_used) return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_used; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1] + (seq_t)6 * cache[i-2];
    }
    cache_used = n + 1;

    return cache[n];
}

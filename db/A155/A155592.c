/*
 * A155592  a(n) = 8^n+2^n-1^n.
 * https://oeis.org/A155592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n + 2^n - 1 a(0) = 1, a(1) = 9 Recurrence: a(n) =
 *            10*a(n-1) - 16*a(n-2) - 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n < cache_len)
        return cache[n];

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;        /* OOM fallback */
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize base cases if needed */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)9;
        cache_len = 2;
    }

    /* Compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t val = (seq_t)10 * prev1 - (seq_t)16 * prev2 - (seq_t)7;
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}

/*
 * A083588  Binomial transform of A083587.
 * https://oeis.org/A083588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 11*a(n-1) - 43*a(n-2) + 69*a(n-3) - 36*a(n-4)
 * Initial values: a(0)=1, a(1)=5, a(2)=19, a(3)=71 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initial cache with first four terms */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory: return zero as fallback */
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 19;
        cache[3] = 71;
        cache_len = 4;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* out of memory: return zero as fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1] * (seq_t)11;
        val -= cache[i-2] * (seq_t)43;
        val += cache[i-3] * (seq_t)69;
        val -= cache[i-4] * (seq_t)36;
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}

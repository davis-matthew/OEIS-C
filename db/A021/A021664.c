/*
 * A021664  Expansion of 1/((1-x)(1-3x)(1-8x)(1-11x)).
 * https://oeis.org/A021664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 167*a(n-2) + 409*a(n-3) - 264*a(n-4), a(0)=1,
 *            a(1)=23, a(2)=362, a(3)=4894
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* allocated slots */
    static idx_t used_len    = 0;   /* number of entries that have been filled */

    /* Ensure that the array is large enough to store up to index n. */
    if (n >= cache_size) {
        idx_t new_size = cache_size ? cache_size << 1 : 1;
        while (new_size <= n) new_size <<= 1;
        if (new_size < 4) new_size = 4;          /* space for the four base terms */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                      /* out‑of‑memory */
        cache      = tmp;
        cache_size = new_size;
    }

    /* Initialise the first four terms (only once). */
    if (used_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)362;
        cache[3] = (seq_t)4894;
        used_len = 4;
    }

    /* Build up the cache using the recurrence until we reach n. */
    for (idx_t i = used_len; i <= n; ++i) {
        seq_t v1 = mul_small(cache[i-1], 23);
        seq_t v2 = mul_small(cache[i-2], 167);
        seq_t v3 = mul_small(cache[i-3], 409);
        seq_t v4 = mul_small(cache[i-4], 264);

        seq_t val = v1;
        val -= v2;
        val += v3;
        val -= v4;
        cache[i] = val;
    }
    if (n + 1 > used_len) used_len = n + 1;

    return cache[n];
}

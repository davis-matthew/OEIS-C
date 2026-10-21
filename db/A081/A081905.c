/*
 * A081905  Third binomial transform of binomial(n+6, 6).
 * https://oeis.org/A081905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            4^n*(n^6+129*n^5+5845*n^4+115215*n^3+993874*n^2+3308616*n+2949120)/2949120
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* 7 initial terms of the sequence */
    static const seq_t init_vals[7] = {
        (seq_t)1,
        (seq_t)10,
        (seq_t)79,
        (seq_t)552,
        (seq_t)3567,
        (seq_t)21810,
        (seq_t)127905
    };

    /* For small n use the table directly */
    if (n < 7)
        return init_vals[(size_t)n];

    /* static cache that grows to the largest n requested */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;      /* number of elements currently stored */

    size_t needed = (size_t)(n + 1);
    if (needed < 7)  needed = 7;      /* safety, though n >= 7 here */

    /* whether we must start computation from a previous length */
    idx_t start_idx = 0;

    if (cache == NULL) {
        /* first allocation */
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;           /* allocation failure unlikely in tests */
        /* initialise first seven terms */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
        cache_len = 7;
        start_idx = 7;
    } else if (needed > (size_t)cache_len) {
        /* enlarge existing cache */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;
        cache = tmp;
        /* init any missing first seven terms if cache was too short before */
        if (cache_len < 7) {
            for (size_t i = (size_t)cache_len; i < 7; ++i)
                cache[i] = init_vals[i];
        }
        start_idx = cache_len;  /* continue from previously stored length */
        cache_len = needed;
    } else {
        /* cache already large enough */
        start_idx = cache_len;
    }

    /* fill in missing entries using the seventh‑order recurrence */
    for (idx_t i = start_idx; i <= n; ++i) {
        seq_t pos = mul_small(cache[i - 1], 28) +
                    mul_small(cache[i - 3], 2240) +
                    mul_small(cache[i - 5], 21504) +
                    mul_small(cache[i - 7], 16384);

        seq_t neg = mul_small(cache[i - 2], 336) +
                    mul_small(cache[i - 4], 8960) +
                    mul_small(cache[i - 6], 28672);

        cache[i] = pos - neg;
    }

    return cache[(size_t)n];
}

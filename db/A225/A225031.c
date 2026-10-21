/*
 * A225031  Non-crossing, non-nesting, 5-colored set partitions.
 * https://oeis.org/A225031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41*a(n‑1) – 638*a(n‑2) + 4701*a(n‑3) – 16398*a(n‑4) +
 *            21721*a(n‑5) – a(n‑6) for n ≥ 6 Initial values: a(0)=1, a(1)=6,
 *            a(2)=41, a(3)=321, a(4)=2846, a(5)=27961
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* base values, kept in a constant array for easy copying */
    static const seq_t base[6] = {
        (seq_t)1,
        (seq_t)6,
        (seq_t)41,
        (seq_t)321,
        (seq_t)2846,
        (seq_t)27961
    };

    if (n >= cache_len) {
        /* enlarge cache to hold indices up to n */
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) exit(1);            /* allocation failed */
        cache = new_cache;

        /* initialise base values inside the new portion of the array */
        for (idx_t i = 0; i < 6 && i < new_len; ++i)
            cache[i] = base[i];

        /* compute missing terms using the recurrence */
        idx_t start = (cache_len < 6) ? 6 : cache_len;
        for (idx_t i = start; i < new_len; ++i) {
            seq_t sum1 = cache[i-1] * (seq_t)41
                       + cache[i-3] * (seq_t)4701
                       + cache[i-5] * (seq_t)21721;
            seq_t sum2 = cache[i-2] * (seq_t)638
                       + cache[i-4] * (seq_t)16398
                       + cache[i-6];
            cache[i] = sum1 - sum2;
        }

        cache_len = new_len;
    }

    return cache[n];
}

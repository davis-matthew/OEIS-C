/*
 * A129888  Start with 10; write down the sum of its digits; add last two
 *          terms; repeat.
 * https://oeis.org/A129888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 10; For n >= 2: if n is even: a(n) = digit_sum(a(n-1));
 *            if n is odd: a(n) = a(n-2) + a(n-1); This implements the OEIS
 *            sequence A129888
 * Range:     u8 n <= 54, u16 n <= 6778, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Cache previously computed terms (static across calls). */
    static seq_t *cache   = NULL;  /* storage for computed terms */
    static size_t cache_size = 0;  /* number of terms currently cached (i.e. cache_size == highest idx stored +1) */
    static size_t cache_cap  = 0;  /* allocated capacity of cache array */

    if (n == 0) return 0;  /* guard against invalid call; offset starts at 1 */

    /* Target number of terms we must have in cache. */
    size_t target = (size_t)n;

    /* Grow cache if necessary. */
    if (cache_size < target) {
        /* Ensure enough capacity. */
        if (cache_cap < target) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while (new_cap < target) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* out-of-memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms. */
        for (size_t i = cache_size; i < target; ++i) {
            seq_t val;
            /* i is 0-based index; term number = i+1. */
            if (i == 0) {                 /* a(1) */
                val = 10;
            } else if (((i + 1) & 1) == 0) { /* even n: a(n) = digit_sum(a(n-1)) */
                val = digit_sum(cache[i - 1]);
            } else {                      /* odd n: a(n) = a(n-2) + a(n-1) */
                val = cache[i - 1] + cache[i - 2];
            }
            cache[i] = val;
        }
        cache_size = target;
    }
    return cache[n - 1];
}

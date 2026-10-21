/*
 * A063280  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          7 ).
 * https://oeis.org/A063280
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 Initial values (for
 *            2 ≤ n ≤ 13): a(2) = -1, a(3) = 0, a(4) = 1, a(5) = 3, a(6) = 5,
 *            a(7) = 7, a(8) = 9, a(9) = 11, a(10) = 13, a(11) = 15, a(12) =
 *            17, a(13) = 17 The function memoizes all computed values in a
 *            static array, so calls can arrive in any order
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_max = 13;          /* highest index guaranteed computed */
    static int init_done = 0;

    /* one‑time static initialisation of the base values */
    if (!init_done) {
        /* ensure room for indices 0 … 13 (size 14) */
        cache_cap = 14;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < cache_cap; ++i)
            cache[i] = (seq_t)0;   /* initialise everything to zero */
        /* set the known terms */
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)3;
        cache[6] = (seq_t)5;
        cache[7] = (seq_t)7;
        cache[8] = (seq_t)9;
        cache[9] = (seq_t)11;
        cache[10] = (seq_t)13;
        cache[11] = (seq_t)15;
        cache[12] = (seq_t)17;
        cache[13] = (seq_t)17;
        init_done = 1;
    }

    /* grow the cache so that index n exists */
    if ((idx_t)(n) >= cache_cap) {
        idx_t new_cap = (idx_t)(n) + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return a safe value (zero) */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;
        /* initialise newly added entries to zero (they are unused until computed) */
        for (idx_t i = cache_max + 1; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
    }

    /* compute any missing terms up to n using the recurrence */
    if ((idx_t)n > cache_max) {
        for (idx_t i = cache_max + 1; i <= (idx_t)n; ++i) {
            if (i <= 13) {
                /* values already set during initialisation */
                continue;
            }
            cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
        }
        cache_max = (idx_t)n;
    }

    return cache[n];
}

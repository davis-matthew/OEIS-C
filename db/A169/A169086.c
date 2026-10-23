/*
 * A169086  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of reduced words of length n in the Coxeter *
 *            group on 25 generators with relations (S_i)^2 = (S_i S_j)^25 =
 *            I. * The sequence satisfies * * a(0) = 1 * a(n) = 25·24^{n-1}
 *            for 1 ≤ n ≤ 24 * a(n) = 23·(a(n-1)+⋯+a(n-24)) - 276·a(n-25) for
 *            n ≥ 25 * * This recurrence is derived from the r...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = 0;          /* last index for which cache[i] is defined */
    static seq_t sum24 = 0;                  /* running sum of the last 24 terms (for n ≥ 25) */
    static int sum24_ready = 0;              /* 0 until n=25 has been computed */
    static seq_t pow24 = 1;                  /* 24^{k} for the direct formula up to n=24 */

    /* ----- ensure cache buffer is large enough ----- */
    if (cache == NULL) {
        cache_size = 64;
        cache = seq_alloc_new_array(cache_size);
        cache[0] = seq_one();      /* a(0) = 1 */
        last_computed = 0;
    }
    if (n >= cache_size) {
        size_t new_cap = ((size_t)n + 16) * 2;
        seq_t *tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        seq_free_array(cache);
        cache = tmp;
        cache_size = new_cap;
    }

    /* ----- compute all terms up to n ----- */
    while (last_computed < n) {
        idx_t i = last_computed + 1;

        if (i <= 24) {
            /* direct power formula */
            cache[i] = (seq_t)25 * pow24;
            pow24 = pow24 * (seq_t)24;
        } else {
            if (!sum24_ready) {
                /* initialise sum24 for i == 25 */
                sum24 = 0;
                for (idx_t k = 1; k <= 24; ++k)
                    sum24 = sum24 + cache[i - k];
                sum24_ready = 1;
            } else {
                /* slide the window: add a[i-1], remove a[i-25] */
                sum24 = sum24 + cache[i - 1] - cache[i - 25];
            }
            cache[i] = (seq_t)23 * sum24 - (seq_t)276 * cache[i - 25];
        }

        last_computed = i;
    }

    return cache[n];
}

/*
 * A169118  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The generating function of A169118 is (t^26 + 2 Σ_{k=1}^{25} t^k
 *            + 1) / (28*t^26 - 7*Σ_{k=1}^{25} t^k + 1). This yields the
 *            linear recurrence a(0) = 1, a(n) = 7·Σ_{k=0}^{n-1} a(k) + 2 for
 *            1 ≤ n ≤ 25, a(26)= 7·Σ_{k=1}^{25} a(26−k) − 28·a(0) + 1, a(n) =
 *            7·Σ_{k=1}^{25} a(n−k) − 28·a(n−26) for n ≥ 27
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /*  static cache for memoisation */
    static seq_t *cache   = NULL;   /* stored values of a(i) */
    static size_t cache_cap = 0;    /* allocated capacity (elements) */
    static idx_t  last_computed = (idx_t)-1; /* largest index stored */

    /* helper to initialise the first 26 values by the recurrence for n≤25 */
    if (cache_cap < 26) {
        size_t new_cap = 26;
        if (!cache) {
            cache = alloc_seq_array(new_cap);
        } else {
            cache = seq_realloc(cache, cache_cap, new_cap);
        }
        cache_cap = new_cap;

        /* initialise a(0)…a(25) */
        cache[0] = (seq_t)1;
        seq_t sum = cache[0];          /* sum of a(0)…a(i-1) during loop */
        for (size_t i = 1; i < 26; ++i) {
            cache[i] = 7 * sum + (seq_t)2;
            sum += cache[i];
        }
        last_computed = 25;
    }

    /* if the requested term is already computed, return it */
    if (n <= last_computed) {
        return cache[(size_t)n];
    }

    /* enlarge cache to at least n+1 elements */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (!cache) {
            cache = alloc_seq_array(new_cap);
        } else {
            cache = seq_realloc(cache, cache_cap, new_cap);
        }
        cache_cap = new_cap;
    }

    /* compute terms from last_computed+1 up to n */
    /* maintain a rolling sum of the last 25 values:
         sum = a(i-1)+…+a(i-25)   (for i ≥ 26)                     */
    seq_t window_sum = (seq_t)0;
    if (last_computed + 1 <= 25) {
        /* we need to compute the window sum for i=26 */
        /* after the initial block, window_sum must be a(1)+…+a(25) */
        window_sum = (seq_t)0;
        for (size_t k = 1; k <= 25; ++k) window_sum += cache[k];
    }

    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 25) {
            /* These values were already computed in the block above */
            continue;
        }

        /* update window_sum for this i before computing a(i) if i>26 */
        if (i > 26) {
            /* sliding window: remove a(i-26), add a(i-1) */
            window_sum = window_sum - cache[i - 26] + cache[i - 1];
        }

        /* recurrence */
        if (i == 26) {
            val = 7 * window_sum - 28 * cache[0] + (seq_t)1;
        } else {
            val = 7 * window_sum - 28 * cache[i - 26];
        }

        cache[i] = val;
    }

    last_computed = (idx_t)n;
    return cache[(size_t)n];
}

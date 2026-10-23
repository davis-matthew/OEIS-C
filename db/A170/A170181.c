/*
 * A170181  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) obeys the linear recurrence a(n) = g(n) + 26·∑_{k=1}^{38}
 *            a(n−k) – 351·a(n−39), where g(0)=g(39)=1, g(1..38)=2, and g(n)=0
 *            for n>39. Equivalent: a(n) + Σ_{k=1}^{39} d_k a(n−k) = g(n) with
 *            d_k = –26 for 1≤k≤38, d_39 = 351, d_0 = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of terms stored so far */

    if (n + 1 > cache_size) {
        /* enlarge cache to size n+1, preserve old values */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
        if (!new_cache) return 0;  /* out of memory; caller may handle */

        if (cache) {
            memcpy(new_cache, cache, (size_t)cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;

        /* compute terms from old cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;

            /* g(i) */
            if (i == 0 || i == 39)
                sum = 1;
            else if (i <= 38)
                sum = 2;

            /* add contributions from a[i‑k] for 1≤k≤39 */
            for (idx_t k = 1; k <= 39; ++k) {
                if (i >= k) {
                    seq_t prev = cache[i - k];
                    if (k < 39)          /* d_k = –26 → +26·prev */
                        sum += 26 * prev;
                    else                 /* k == 39, d_k = 351 → –351·prev */
                        sum -= 351 * prev;
                }
            }
            cache[i] = sum;  /* d_0 = 1, so a[i] = sum */
        }
        cache_size = n + 1;
    }
    return cache[n];
}

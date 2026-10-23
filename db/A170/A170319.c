/*
 * A170319  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function
   a(0) = 1
   for 1 ≤ n ≤ 41:       a(n) = 2 + 20 * Σ_{k=0}^{n-1} a(k)
   a(42) = 1 - 210*a(0) + 20 * Σ_{k=1}^{41} a(k)
   for n ≥ 43:           a(n) = 20 * Σ_{k=n-41}^{n-1} a(k) - 210 * a(n-42)
*/
#include <stdlib.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;   /* allocated size (number of terms)      */
    static size_t cache_len = 0;   /* number of terms actually stored   */

    /* running sums needed for the recurrence */
    static seq_t sum_all    = 0;   /* Σ_{k=0}^{i-1} a(k)  (updated after each term) */
    static seq_t sum_last41 = 0;   /* Σ_{k = i-41}^{i-1} a(k)  (updated after each term) */

    /* allocate or grow the cache as needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) abort();      /* out‑of‑memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;

            if (i == 0) {
                val = (seq_t)1;
            } else if (i <= 41) {
                val = (seq_t)2 + (seq_t)20 * sum_all;
            } else {
                /* before computing a(i) adjust the window for the recurrence */
                if (i >= 42) {
                    sum_last41 -= cache[i - 42];
                }

                if (i == 42) {
                    val = (seq_t)1
                          - (seq_t)210 * cache[0]
                          + (seq_t)20 * sum_last41;
                } else {        /* i >= 43 */
                    val = (seq_t)20 * sum_last41
                          - (seq_t)210 * cache[i - 42];
                }
            }

            cache[i] = val;
            sum_all += val;
            sum_last41 += val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}

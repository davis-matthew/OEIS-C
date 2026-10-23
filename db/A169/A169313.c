/*
 * A169313  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 10 * sum_{k=0}^{n-1} a(k) for n < 30 a(n) = 10 *
 *            sum_{k=1}^{29} a(n-k) - 55 * a(n-30) for n >= 30
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t sum_tot   = 0; /* sum of a[0..i-1] for i < 30  */
    static seq_t window_sum = 0;/* sum of the last 29 terms for i >=30 */

    /* Ensure cache large enough */
    size_t required = (size_t)n + 1;
    if (required > cap) {
        size_t new_cap = cap ? cap << 1 : 32;
        while (new_cap < required)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = len; i < required; ++i) {
        seq_t val;
        if (i < 30) {
            seq_t p = (i == 0) ? 1 : 2;
            val = p + sum_tot * (seq_t)10;
            sum_tot += val;
        } else {
            if (i == 30) {
                /* initialise the 29‑term window with a[1]..a[29] */
                window_sum = 0;
                for (size_t j = 1; j < 30; ++j)
                    window_sum += cache[j];
            }
            val = window_sum * (seq_t)10 - cache[i - 30] * (seq_t)55;
            /* update the sliding window for the next coefficient */
            window_sum += val;
            window_sum -= cache[i - 29];
        }
        cache[i] = val;
    }

    len = required;
    return cache[(size_t)n];
}

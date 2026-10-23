/*
 * A015310  Gaussian binomial coefficient [ n,5 ] for q = -6.
 * https://oeis.org/A015310
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [n,5]q with q = -6 (Gaussian binomial coefficient)
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 5)   /* values below the offset are irrelevant */
        return 0;

    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max_n = 0;

    static seq_t *pow_arr = NULL;
    static size_t pow_cap = 0;
    static idx_t pow_max = 0;

    static seq_t prev_row[6];
    static seq_t curr_row[6];
    static idx_t computed_n = 0;
    static int initialised = 0;

    if (!initialised) {
        initialised = 1;
        /* initialise row for n = 0 */
        prev_row[0] = 1;
        for (int i = 1; i < 6; ++i)
            prev_row[i] = 0;

        /* power array with one element for exponent 0 */
        pow_cap = 1;
        pow_arr = (seq_t*)malloc(sizeof(seq_t) * pow_cap);
        if (!pow_arr) abort();
        pow_arr[0] = 1;
        pow_max = 0;
    }

    /* extend pre‑computed powers if necessary */
    if (pow_cap <= (size_t)n) {
        size_t new_cap = pow_cap ? pow_cap * 2 : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(pow_arr, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        pow_arr = tmp;
        pow_cap = new_cap;
    }
    for (idx_t i = pow_max + 1; i <= n; ++i)
        pow_arr[i] = pow_arr[i - 1] * (seq_t)-6;
    pow_max = n;

    /* compute new rows up to requested n */
    for (idx_t m = computed_n + 1; m <= n; ++m) {
        curr_row[0] = 1;
        int maxk = (m < 5 ? (int)m : 5);
        for (int k = 1; k <= maxk; ++k) {
            if (k == m)
                curr_row[k] = 1;
            else
                curr_row[k] = prev_row[k] + pow_arr[m - k] * prev_row[k - 1];
        }
        for (int k = maxk + 1; k <= 5; ++k)
            curr_row[k] = 0;

        /* store coefficient if we reached k = 5 */
        if (m >= 5) {
            if (cache_cap <= (size_t)m) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                while (new_cap <= (size_t)m) new_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[m] = curr_row[5];
            if (cache_max_n < m) cache_max_n = m;
        }

        /* prepare for next iteration */
        for (int k = 0; k <= 5; ++k)
            prev_row[k] = curr_row[k];
    }
    computed_n = n;

    return cache[n];
}

/*
 * A075924  Fifth column of triangle A075502.
 * https://oeis.org/A075924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n * S2(n+5,5) (Stirling second‑kind)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation of a(n) */
    static seq_t *a_cache = NULL;
    static size_t a_size = 0;

    /* 7^n values */
    static seq_t *pow_cache = NULL;
    static size_t pow_size = 0;

    /* Stirling numbers S(m,5) for m = n+5 */
    static seq_t *s5_cache = NULL;
    static size_t s5_size = 0;

    /* Current and previous Stirling rows (k = 0..5) */
    static unsigned __int128 stirling_prev[6] = {0};
    static idx_t          stirling_max   = 0;

    /*--- Ensure a_cache is large enough ---*/
    if (a_size <= n) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        if (a_size < new_size) {                 /* initialise new entries */
            for (size_t i = a_size; i < new_size; ++i)
                tmp[i] = 0;
        }
        a_cache = tmp;
        a_size = new_size;
    }
    if (a_cache[n] != 0)
        return a_cache[n];

    /*--- Compute powers of 7 up to 7^n ---*/
    if (pow_size == 0) {                          /* first allocation   */
        pow_cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!pow_cache) exit(1);
        pow_cache[0] = (seq_t)1;
        pow_size   = 1;
    }
    if (pow_size <= n) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(pow_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        for (size_t i = pow_size; i <= n; ++i)
            tmp[i] = tmp[i-1] * (seq_t)7;
        pow_cache = tmp;
        pow_size  = new_size;
    }

    /*--- Ensure s5_cache is large enough ---*/
    idx_t target_m = n + 5;
    if (s5_size <= target_m) {
        size_t new_size = target_m + 1;
        seq_t *tmp = (seq_t *)realloc(s5_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        if (s5_size < new_size)
            for (size_t i = s5_size; i < new_size; ++i)
                tmp[i] = 0;
        s5_cache = tmp;
        s5_size  = new_size;
    }

    /*--- Initialise Stirling base row if necessary ---*/
    if (stirling_max == 0) {
        for (int k = 0; k <= 5; ++k)
            stirling_prev[k] = 0;
        stirling_prev[0] = 1;
        stirling_max     = 0;
    }

    /*--- Compute Stirling rows up to target_m ---*/
    for (idx_t m = stirling_max + 1; m <= target_m; ++m) {
        unsigned __int128 stirling_cur[6] = {0};
        stirling_cur[0] = 0;
        for (int k = 1; k <= 5; ++k)
            stirling_cur[k] = (unsigned __int128)k * stirling_prev[k] + stirling_prev[k-1];

        if (m >= 5)
            s5_cache[m] = (seq_t)stirling_cur[5];

        for (int k = 0; k <= 5; ++k)
            stirling_prev[k] = stirling_cur[k];
    }
    stirling_max = target_m;

    /*--- Compute the term ---*/
    unsigned __int128 prod = (unsigned __int128)pow_cache[n] *
                             (unsigned __int128)s5_cache[target_m];
    seq_t val = (seq_t)prod;
    a_cache[n] = val;
    return val;
}

/*
 * A126415  Number of base 28 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of base‑28 n‑digit numbers with adjacent digits differing by at most 2
 * (leading zeros allowed).  The value a(n) is the total number of sequences
 * of length n over digits 0…27 where |d_i − d_{i+1}| ≤ 2.  This is computed
 * by dynamic programming:  dp_k[d] is the number of length‑k sequences ending
 * with digit d.  Then dp_{k+1}[j] = ∑_{i:|i−j|≤2} dp_k[i}.
 * a(n) = ∑_d dp_n[d]  with dp_1[d] = 1 for all d.  The function memoises
 * computed results so that any number of calls in any order keep the same
 * linear‑time cost per maximum n requested.  Max n ⩽ 10 000.  All arithmetic
 * uses type seq_t, which may be any unsigned integral or arbitrary‑precision
 * type. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static state variables for memoization */
    static seq_t *cache = NULL;          /* array of a values */
    static size_t cache_size = 0;        /* allocation size */
    static idx_t cached_n = 0;          /* largest n for which cache[n] is valid */
    static seq_t dp_cur[28];            /* dp for current length */
    static int dp_valid = 0;            /* 0 if dp_cur not initialised */

    /* initialise cache for a(0) */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;            /* a(0) = 1 (empty sequence) */
    }

    if (n == 0)
        return cache[0];

    /* initialise dp for length 1 if needed */
    if (!dp_valid) {
        for (idx_t d = 0; d < 28; ++d)
            dp_cur[d] = (seq_t)1;           /* each digit can start a sequence */
        seq_t total = 0;
        for (idx_t d = 0; d < 28; ++d)
            total += dp_cur[d];
        if (cached_n + 1 >= cache_size) {
            size_t new_size = cache_size * 2;
            if ((size_t)(cached_n + 1) + 1 > new_size)
                new_size = (size_t)(cached_n + 1) + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_size = new_size;
        }
        cache[1] = total;            /* a(1) = 28 */
        cached_n = 1;
        dp_valid = 1;
    }

    /* if requested n already computed, return it */
    if (n <= cached_n)
        return cache[n];

    /* compute values up to n */
    static seq_t dp_next[28];
    for (idx_t len = cached_n + 1; len <= n; ++len) {
        for (idx_t j = 0; j < 28; ++j) {
            seq_t sum = 0;
            idx_t lo = (j >= 2) ? j - 2 : 0;
            idx_t hi = (j <= 25) ? j + 2 : 27;
            for (idx_t i = lo; i <= hi; ++i)
                sum += dp_cur[i];
            dp_next[j] = sum;
        }
        /* compute total for this length */
        seq_t total = 0;
        for (idx_t j = 0; j < 28; ++j)
            total += dp_next[j];

        /* extend cache if necessary */
        if (len >= cache_size) {
            size_t new_size = cache_size * 2;
            if ((size_t)len + 1 > new_size)
                new_size = (size_t)len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_size = new_size;
        }
        cache[len] = total;
        /* prepare for next iteration */
        for (idx_t j = 0; j < 28; ++j)
            dp_cur[j] = dp_next[j];
    }
    cached_n = n;
    return cache[n];
}

/*
 * A167670  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A167670 – number of reduced words of length n in the Coxeter group
    on 14 generators with (S_i)^2 = (S_i S_j)^15 = I.
    
    Recurrence (derived from the rational generating function)
        a(0) = 1
        for n ≥ 1:
            a(n) = 12·(a(n-1)+a(n-2)+…+a(n-14))
                   – 78·a(n-15) + c(n)
            where
                c(0) = 1,   c(1)…c(14) = 2,   c(15) = 1,   c(n)=0 for n>15.
    The sequence starts: 1, 14, 182, 2366, 30758, 399854, …   */
/*  The implementation computes a(n) iteratively, memoising all
    values up to the maximum n ever requested, in time O(n).  */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated size */
    static idx_t  cache_len = 0;          /* number of values stored (next free index) */

    /* Ensure enough space for n+1 values */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < required)
            new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – abort.  The tests never trigger this. */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise values that have not yet been computed */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;

        /* Sum 12·a(i-k) for k = 1 … 14 */
        for (int k = 1; k <= 14; ++k) {
            if (i >= (size_t)k)
                val += cache[i - k] * (seq_t)12;
        }

        /* Subtract 78·a(i-15) if defined */
        if (i >= 15)
            val -= cache[i - 15] * (seq_t)78;

        /* Add the numerator coefficient c(i) */
        seq_t c;
        if (i == 0 || i == 15)
            c = 1;
        else if (i >= 1 && i <= 14)
            c = 2;
        else
            c = 0;
        val += c;

        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}

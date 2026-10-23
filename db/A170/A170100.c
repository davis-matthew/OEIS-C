/*
 * A170100  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A170100
 *
 *  a(0) = 1
 *  For 1 ≤ n ≤ 36:  p_n = 2
 *  n = 37          :  p_n = 1
 *  n ≥ 38          :  p_n = 0
 *
 *  Recurrence (derived from the rational generating function)
 *      a(n) = p_n + 41 * Σ_{i=1}^{36} a(n-i) - 861 * a(n-37)
 *
 *  The first 37 values are obtained by evaluating this recurrence for
 *  n = 0 … 36, with a(n-i) taken as 0 for n-i < 0.  Once a(0)…a(36)
 *  are known, the homogeneous part of the recurrence applies for all
 *  n ≥ 37.
 *
 *  The function uses memoisation so that arbitrary indices can be
 *  requested in any order with O(max(n)) time overall.
 */

#include <stdlib.h>  /* malloc, realloc */

static seq_t *a_cache = NULL;      /* memoised values a(0 .. last_index) */
static idx_t  a_len   = 0;          /* number of entries allocated */
static idx_t  a_last  = (idx_t)-1;  /* index of last computed value */

seq_t a(idx_t n) {
    /* Ensure cache has at least n+1 entries */
    if (a_len == 0) {
        a_len = 1;
        a_cache = (seq_t*)malloc(a_len * sizeof(seq_t));
        a_cache[0] = (seq_t)1;     /* a(0) = 1 */
        a_last = 0;
    }

    if (n >= a_len) {
        /* Grow cache to n+1 entries */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed – return 0 */
        a_cache = tmp;
        a_len   = new_len;

        /* Compute new values from a_last+1 up to n */
        for (idx_t i = a_last + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            /* 41 * Σ a[i-j] for j = 1 .. 36, if i >= j */
            for (idx_t j = 1; j <= 36 && i >= j; ++j) {
                sum += (seq_t)41 * a_cache[i - j];
            }
            /* -861 * a[i-37] if i >= 37 */
            if (i >= 37) {
                sum -= (seq_t)861 * a_cache[i - 37];
            }

            /* Determine p_i */
            seq_t p;
            if (i == 0)           p = (seq_t)1;
            else if (i <= 36)     p = (seq_t)2;
            else if (i == 37)     p = (seq_t)1;
            else                  p = (seq_t)0;

            a_cache[i] = sum + p;
        }
        a_last = n;
    }

    return a_cache[n];
}

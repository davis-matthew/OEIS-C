/*
 * A170320  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * For 1 <= n <= 41 : a(n) = 2 + 21 * Σ_{k=0}^{n-1} a(k)
 *            * For n >= 42 : a(n) = 21 * ( Σ_{k=n-41}^{n-1} a(k) ) - 231 *
 *            a(n-42) * * The values are computed once and cached. The cache
 *            is stored in * static arrays that grow only when a larger index
 *            is requested. * All arithmetic uses the typ...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */

seq_t a(idx_t n)
{
    /* static cache of computed terms and their prefix sums */
    static seq_t* vals   = NULL;   /* a(0)..a(max)          */
    static seq_t* pref   = NULL;   /* prefix sums S(i)=∑_{0..i}a(k) */
    static size_t len = 0;         /* number of cached elements (max index+1) */

    /* Ensure we have room for index n */
    if (n >= (idx_t)len) {
        size_t new_len = (size_t)n + 1;
        seq_t* new_vals = (seq_t*)realloc(vals, new_len * sizeof(seq_t));
        seq_t* new_pref = (seq_t*)realloc(pref, new_len * sizeof(seq_t));
        if (!new_vals || !new_pref) abort(); /* allocation failure */

        vals = new_vals;
        pref = new_pref;

        /* If the cache was empty, initialise the very first term */
        if (len == 0) {
            vals[0] = (seq_t)1;           /* a(0) = 1 */
            pref[0] = vals[0];             /* prefix sum S(0) = 1 */
            len = 1;
        }

        /* Compute all terms up to the required index */
        for (size_t i = len; i < new_len; ++i) {
            if (i <= 41) {                 /* 1 <= i <= 41  */
                seq_t sum = pref[i-1];        /* Σ_{0..i-1} a(k)      */
                vals[i] = (seq_t)2 + (seq_t)21 * sum;
            } else {                       /* i >= 42          */
                seq_t sum_diff = pref[i-1] - pref[i-42]; /* Σ_{i-41}^{i-1} a(k) */
                seq_t term1 = (seq_t)21 * sum_diff;
                seq_t term2 = vals[i-42] * (seq_t)231;
                vals[i] = term1 - term2;
            }
            pref[i] = pref[i-1] + vals[i];
        }
        len = new_len;
    }

    return vals[(size_t)n];
}

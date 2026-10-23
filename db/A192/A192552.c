/*
 * A192552  a(n) = Sum_{k=0..n} Stirling2(n,k)*(-1)^(n-k)*k!^2.
 * https://oeis.org/A192552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k)*(-1)^(n-k)*k!^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state */
    static seq_t *a_vals   = NULL;   /* sequence values */
    static seq_t *fact     = NULL;   /* factorials k!   */
    static seq_t *fact2    = NULL;   /* (k!)^2          */
    static seq_t *S_prev   = NULL;   /* Stirling row n-1 */
    static seq_t *S_curr   = NULL;   /* Stirling row n   */
    static size_t buf_size = 0;      /* allocated size   */
    static idx_t   max_computed = (idx_t)-1;   /* last n computed */

    /* --- initialization ---------------------------------------------------- */
    if (max_computed == (idx_t)-1) {
        buf_size = 1;
        a_vals   = (seq_t*)malloc(buf_size * sizeof(seq_t));
        fact     = (seq_t*)malloc(buf_size * sizeof(seq_t));
        fact2    = (seq_t*)malloc(buf_size * sizeof(seq_t));
        S_prev   = (seq_t*)malloc(buf_size * sizeof(seq_t));
        S_curr   = (seq_t*)malloc(buf_size * sizeof(seq_t));

        /* a(0) = 1 */
        a_vals[0] = 1;
        fact[0]   = 1;
        fact2[0]  = 1;
        S_prev[0] = 1;                /* S(0,0)=1 */

        max_computed = 0;
    }

    /* --- ensure enough storage ------------------------------------------- */
    if ((size_t)(n+1) > buf_size) {
        size_t new_len = (size_t)(n + 1);
        a_vals = (seq_t*)realloc(a_vals,  new_len * sizeof(seq_t));
        fact   = (seq_t*)realloc(fact,    new_len * sizeof(seq_t));
        fact2  = (seq_t*)realloc(fact2,   new_len * sizeof(seq_t));
        S_prev = (seq_t*)realloc(S_prev,  new_len * sizeof(seq_t));
        S_curr = (seq_t*)realloc(S_curr,  new_len * sizeof(seq_t));
        buf_size = new_len;
    }

    /* --- compute missing terms ------------------------------------------- */
    if (n > max_computed) {
        /* iterate from the next index up to n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* factorials */
            fact[i]  = fact[i-1] * (seq_t)i;          /* k!   */
            fact2[i] = fact[i] * fact[i];             /* (k!)^2 */

            /* Stirling numbers of second kind for row i */
            S_curr[0] = 0;                            /* S(i,0)=0 for i>0 */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t left  = S_prev[k-1];
                seq_t right = 0;
                if (k < i)                      /* S_prev[k] exists only for k<i */
                    right = S_prev[k] * (seq_t)k;  /* k * S(prev,k) */
                S_curr[k] = left + right;
            }

            /* compute sums of even and odd k */
            seq_t sum_even = 0;
            seq_t sum_odd  = 0;
            for (idx_t k = 0; k <= i; ++k) {
                seq_t term = S_curr[k] * fact2[k];
                if ((k & 1) == 0)
                    sum_even += term;
                else
                    sum_odd  += term;
            }

            /* final a(i) = sum_even - sum_odd   or   sum_odd - sum_even */
            seq_t a_val;
            if ((i & 1) == 0)  /* even i */
                a_val = sum_even - sum_odd;
            else
                a_val = sum_odd - sum_even;

            a_vals[i] = a_val;

            /* swap rows for next iteration */
            seq_t *tmp = S_prev;
            S_prev = S_curr;
            S_curr = tmp;
        }
        max_computed = n;
    }

    return a_vals[n];
}

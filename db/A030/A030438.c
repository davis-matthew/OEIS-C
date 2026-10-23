/*
 * A030438  a(n) = A030019(n)*n! (or A035051*(n-1)!).
 * https://oeis.org/A030438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{i=0}^{n-1} Stirling2(n-1,i) * n^i (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External definitions supplied by the build system */
extern idx_t _prime_limit_for_idx(idx_t n);

/* Computes A030438(n) */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_vals   = NULL;   /* array holding a(0)…a(current_n)          */
    static size_t   a_len  = 0;      /* current size of a_vals                   */
    static seq_t *s_row   = NULL;   /* current row of Stirling numbers S(r,k)    */
    static idx_t   s_len   = 0;      /* length of s_row (r+1)                    */
    static idx_t   current_n = 0;    /* highest n already computed                */
    static seq_t   fact_cur = 1;     /* (current_n)!                             */

    /* initialise on first call */
    if (a_vals == NULL) {
        a_vals = (seq_t *)malloc(sizeof(seq_t));   /* a(0) */
        if (!a_vals) return (seq_t)0;              /* allocation failed */
        a_vals[0] = 1;
        a_len = 1;

        s_row = (seq_t *)malloc(sizeof(seq_t));    /* S(0,0)=1 */
        if (!s_row) { free(a_vals); return (seq_t)0; }
        s_row[0] = 1;
        s_len = 1;

        current_n = 0;
        fact_cur = 1; /* 0!   */
    }

    if (n == 0)
        return a_vals[0];

    /* compute values up to requested n */
    for (idx_t m = current_n + 1; m <= n; ++m) {
        /* sum_{k=0}^{m-1} S(m-1,k) * m^k   */
        seq_t sum = 0;
        seq_t pow_val = 1;  /* m^0 */
        for (idx_t k = 0; k < m; ++k) { /* s_row has length m (row m-1) */
            seq_t term = s_row[k] * pow_val;
            sum += term;
            pow_val *= (seq_t)m;
        }

        /* a(m) = (m-1)! * sum */
        seq_t a_m = fact_cur * sum;

        /* store a(m) */
        if (a_len <= (size_t)m) {
            size_t new_len = m + 1;
            seq_t *tmp = (seq_t *)realloc(a_vals, sizeof(seq_t) * new_len);
            if (!tmp) { free(a_vals); free(s_row); return (seq_t)0; }
            a_vals = tmp;
            a_len = new_len;
        }
        a_vals[m] = a_m;

        /* update factorial for next iteration: (m)! = (m-1)! * m */
        fact_cur *= (seq_t)m;

        /* compute next Stirling row S(m,k) for k=0..m   */
        size_t new_len = m + 1;
        seq_t *new_row = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (!new_row) { free(a_vals); free(s_row); return (seq_t)0; }

        new_row[0] = 0;  /* S(m,0)=0 for m>0 */
        for (idx_t k = 1; k < m; ++k) {
            /* S(m,k) = k * S(m-1,k) + S(m-1,k-1) */
            seq_t val = (seq_t)k * s_row[k] + s_row[k-1];
            new_row[k] = val;
        }
        new_row[m] = 1;  /* S(m,m)=1 */

        free(s_row);
        s_row = new_row;
        s_len = new_len;
        current_n = m;
    }

    return a_vals[n];
}

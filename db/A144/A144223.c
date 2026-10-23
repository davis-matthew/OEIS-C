/*
 * A144223  Number of ways of placing n labeled balls into n unlabeled (but
 *          6-colored) boxes.
 * https://oeis.org/A144223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 6^k * S(n,k) where S(n,k) are Stirling numbers
 *            of the second kind
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for the computed a(n) values and the two rows of Stirling numbers */
    static seq_t *a_table = NULL;      /* a values */
    static size_t a_cap   = 0;
    static idx_t   a_max  = 0;          /* largest n already computed */

    static seq_t *S_prev = NULL;       /* row n‑1 of S(n‑1,k) */
    static seq_t *S_curr = NULL;       /* row n   of S(n,k)   */
    static size_t S_cap   = 0;          /* capacity of the S arrays */

    /* one time initialisation */
    if (a_table == NULL) {
        a_cap  = 1;
        a_table = (seq_t*)malloc(a_cap * sizeof(seq_t));
        a_table[0] = (seq_t)1;           /* a(0) = 1 */

        a_max  = 0;

        S_cap = 1;
        S_prev = (seq_t*)malloc(S_cap * sizeof(seq_t));
        S_curr = (seq_t*)malloc(S_cap * sizeof(seq_t));
        S_prev[0] = (seq_t)1;           /* S(0,0) = 1 */
    }

    /* grow a_table if needed */
    if ((size_t)n + 1 > a_cap) {
        size_t newcap = a_cap ? a_cap * 2 : 1;
        while (newcap < (size_t)n + 1) newcap *= 2;
        a_table = (seq_t*)realloc(a_table, newcap * sizeof(seq_t));
        a_cap = newcap;
    }

    /* compute missing values up to n */
    for (idx_t i = a_max + 1; i <= n; ++i) {
        /* ensure S arrays have enough space for i+1 columns */
        if ((size_t)i + 1 > S_cap) {
            size_t newcap = S_cap ? S_cap * 2 : 1;
            while (newcap < (size_t)i + 1) newcap *= 2;
            S_prev = (seq_t*)realloc(S_prev, newcap * sizeof(seq_t));
            S_curr = (seq_t*)realloc(S_curr, newcap * sizeof(seq_t));
            S_cap = newcap;
        }

        seq_t sum    = 0;          /* Σ 6^k * S(i,k) */
        seq_t power6 = 1;          /* 6^0 */
        S_curr[0] = 0;             /* S(i,0) = 0 for i>0 */

        /* compute Stirling numbers of the second kind for row i */
        for (idx_t k = 1; k <= i; ++k) {
            /* S(i,k) = S(i-1,k-1) + k*S(i-1,k) */
            seq_t term1 = (k-1 <= i-1) ? S_prev[k-1] : (seq_t)0;
            seq_t term2 = (k   <= i-1) ? (seq_t)k * S_prev[k] : (seq_t)0;
            S_curr[k] = term1 + term2;

            power6 = power6 * (seq_t)6;
            sum    = sum + power6 * S_curr[k];
        }

        a_table[i] = sum;

        /* prepare for next iteration: swap rows */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
    }
    a_max = n;

    return a_table[n];
}

/*
 * A210661  The total number of ways to linearly order the connected
 *          components of each functional digraph over all functions
 *          f:{1,2,...,n}->{1,2,...,n}.
 * https://oeis.org/A210661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} C(n,k) * a(n-k) * conn(k) * where conn(k)
 *            is the number of connected functional digraphs on k nodes
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function below implements the recurrence
 *    a(0) = 1,
 *    conn(1) = 1,
 *    conn(n) = n^n - Σ_{m=1}^{n-1} C(n-1,m-1) * conn(m) * (n-m)^(n-m)
 *    a(n) = Σ_{k=1}^{n} C(n,k) * a(n-k) * conn(k)      (n ≥ 1)
 *
 *  All intermediate values are stored in seq_t, which may be an
 *  arbitrary‑precision integer type.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n) {
    /* ---------- static storage for memoisation ---------- */
    static seq_t *fact    = NULL;   /* factorials: fact[i] = i!   */
    static seq_t *pow_n   = NULL;   /* pow_n[i]  = i^i            */
    static seq_t *conn    = NULL;   /* conn[i]   = number of connected functional digraphs on i nodes */
    static seq_t *ans     = NULL;   /* ans[i]    = a(i)           */
    static idx_t  max_n   = 0;      /* highest n computed so far */

    /* ---------- initialise base case ---------- */
    if (max_n == 0 && fact == NULL) {
        fact   = (seq_t *)malloc(sizeof(seq_t));
        pow_n  = (seq_t *)malloc(sizeof(seq_t));
        conn   = (seq_t *)malloc(sizeof(seq_t));
        ans    = (seq_t *)malloc(sizeof(seq_t));

        fact[0] = (seq_t)1;
        pow_n[0] = (seq_t)1;   /* 0^0 defined as 1 for combinatorial purposes */
        conn[0] = (seq_t)0;    /* not used, but initialise */
        ans[0]  = (seq_t)1;    /* a(0) = 1 */
        max_n = 0;
    }

    /* ---------- extend tables up to n ---------- */
    if (n > max_n) {
        idx_t new_max = n;
        fact   = (seq_t *)realloc(fact,   sizeof(seq_t) * (new_max + 1));
        pow_n  = (seq_t *)realloc(pow_n,  sizeof(seq_t) * (new_max + 1));
        conn   = (seq_t *)realloc(conn,   sizeof(seq_t) * (new_max + 1));
        ans    = (seq_t *)realloc(ans,    sizeof(seq_t) * (new_max + 1));

        /* compute factorials and i^i for new indices */
        for (idx_t i = max_n + 1; i <= new_max; ++i) {
            fact[i]  = fact[i-1] * (seq_t)i;
            pow_n[i] = pow_uint((uint64_t)i, i);
        }

        /* compute conn[i] and ans[i] for i = max_n+1 .. new_max */
        for (idx_t i = max_n + 1; i <= new_max; ++i) {
            /* ---------- compute conn[i] ---------- */
            seq_t sum = 0;
            seq_t binom = (seq_t)1;          /* C(i-1, 0) */
            for (idx_t m = 1; m < i; ++m) {
                /* binom = C(i-1, m-1) */
                sum += binom * conn[m] * pow_n[i - m];
                if (m < i - 1) {
                    binom = binom * ((seq_t)(i - m)) / (seq_t)m;
                }
            }
            conn[i] = pow_n[i] - sum;

            /* ---------- compute ans[i] ---------- */
            seq_t total = 0;
            seq_t binom_k = (seq_t)1;          /* C(i, 0) */
            for (idx_t k = 1; k <= i; ++k) {
                /* binom_k = C(i, k) */
                binom_k = binom_k * ((seq_t)(i - k + 1)) / (seq_t)k;
                total += binom_k * ans[i - k] * conn[k];
            }
            ans[i] = total;
        }
        max_n = new_max;
    }

    return ans[n];
}

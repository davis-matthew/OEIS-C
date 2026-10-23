/*
 * A107883  Column 3 of triangle A107880.
 * https://oeis.org/A107883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A107883: Column 3 of triangle A107880
 *
 * The sequence satisfies
 *
 *    1 = Σ_{k≥0} a(k)·x^k·(1−x)^{((k+2)(k+3)/2−1)}
 *
 * Expanding the powers of (1−x) and collecting the coefficient of x^n
 * gives the recurrence
 *
 *    a(n) = Σ_{k=0}^{n−1} a(k)·C(e_k, n−k)·(−1)^{n−k+1},
 *
 * where e_k = (k^2+5k+4)/2.  The function below memoises the
 * values and updates the binomial coefficients incrementally in
 * O(n^2) time.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_vals  = NULL;   /* a(k) for 0 ≤ k ≤ max_n            */
static seq_t *binom_cur = NULL; /* C(e_k, r) for r = cur−k           */
static idx_t  capacity = 0;     /* allocated array size              */
static idx_t  max_n    = 0;     /* largest n computed so far         */
static bool   init_done = false;/* one‑time initialisation           */

seq_t a(idx_t n)
{
    /* --- one‑time initialization ------------------------------------ */
    if (!init_done) {
        capacity = 1;
        a_vals   = (seq_t *)realloc(NULL, capacity * sizeof(seq_t));
        binom_cur= (seq_t *)realloc(NULL, capacity * sizeof(seq_t));

        a_vals[0]   = (seq_t)1;  /* a(0) = 1                     */
        binom_cur[0]= (seq_t)1;  /* C(e_0, 0) = 1                 */
        max_n = 0;
        init_done = true;
    }

    /* --- extend memoisation up to n --------------------------------- */
    if (n > max_n) {
        /* grow arrays to size n+1 */
        while ((idx_t)(n + 1) > capacity) {
            idx_t new_cap = (capacity == 0) ? 1 : capacity << 1;
            capacity = new_cap;
            a_vals    = (seq_t *)realloc(a_vals, capacity * sizeof(seq_t));
            binom_cur = (seq_t *)realloc(binom_cur, capacity * sizeof(seq_t));
        }

        /* compute terms one by one */
        for (idx_t cur = max_n + 1; cur <= n; ++cur) {

            /* ---- update binomial coefficients for all k<cur -------- */
            for (idx_t k = 0; k < cur; ++k) {
                /* e_k = (k^2 + 5k + 4)/2 */
                seq_t ek = ( ((seq_t)k * (seq_t)k)
                           + ((seq_t)5 * (seq_t)k)
                           + (seq_t)4 ) / (seq_t)2;

                idx_t r = cur - k;            /* current exponent    */

                /* For the newly added k (k = cur-1) we need C(e_k, 1) */
                if (k == cur - 1) {
                    binom_cur[k] = ek;        /* C(e_k, 1)           */
                } else {
                    /* C(e_k, r) = C(e_k, r‑1) * (e_k‑(r‑1)) / r   */
                    seq_t factor = ek - ((seq_t)r - (seq_t)1);
                    binom_cur[k] = (binom_cur[k] * factor) / (seq_t)r;
                }
            }

            /* ---- compute a(cur) via the recurrence ----------------- */
            seq_t sum_odd  = (seq_t)0;
            seq_t sum_even = (seq_t)0;

            for (idx_t k = 0; k < cur; ++k) {
                seq_t term = a_vals[k] * binom_cur[k];
                if ( ((cur - k) & (idx_t)1) == 1 )
                    sum_odd += term;   /* +term  when n-k is odd   */
                else
                    sum_even += term;  /* –term  when n-k is even  */
            }

            a_vals[cur] = sum_odd - sum_even;
        }

        max_n = n;
    }

    return a_vals[n];
}

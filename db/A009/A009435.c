/*
 * A009435  Expansion of e.g.f.: log(1+x)/cosh(x).
 * https://oeis.org/A009435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=1..n} binom(n,k) * (-1)^{k+1}*(k-1)! * g_{n-k} where
 *            g_0 = 1, and g_m satisfies Σ_{i=0..⌊m/2⌋} binom(m,2i)*g_{m-2i} =
 *            0 (m ≥ 1)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* internal storage for the sequence values and the auxiliary g array */
static seq_t *g_arr = NULL;    /* g[0..n]  */
static seq_t *a_arr = NULL;    /* a[0..n]  */
static size_t cap = 0;         /* allocated size of the arrays */
static idx_t computed_n = 0;   /* highest index already computed */

/* ensure capacity for indices up to (need-1) */
static void ensure_cap(size_t need)
{
    if (cap >= need) return;
    size_t new_cap = cap ? cap * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    g_arr  = (seq_t *)realloc(g_arr,  new_cap * sizeof(seq_t));
    a_arr  = (seq_t *)realloc(a_arr,  new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    /* first call: initialise arrays */
    if (computed_n == 0) {
        ensure_cap(1);          /* at least room for index 0 */
        g_arr[0] = 1;
        a_arr[0] = 0;
    }

    /* compute values up to n if necessary */
    if (n > computed_n) {
        ensure_cap((size_t)n + 1);

        for (idx_t m = computed_n + 1; m <= n; ++m) {
            /* ---------- compute g_m ---------- */
            seq_t sum_g = 0;
            seq_t binom_g = 1;                     /* binom(m,0) = 1 */
            idx_t max_i = m >> 1;                  /* floor(m/2) */
            for (idx_t i = 1; i <= max_i; ++i) {
                /* compute binom(m,2i) from binom(m,2(i-1)) */
                binom_g = (binom_g *
                           (seq_t)(m - 2*i + 2) *
                           (seq_t)(m - 2*i + 1)) /
                          ((seq_t)(2*i) * (seq_t)(2*i - 1));
                sum_g += binom_g * g_arr[m - 2*i];
            }
            g_arr[m] = -sum_g;   /* g_m = -sum */

            /* ---------- compute a_m ---------- */
            seq_t sum_a = 0;
            seq_t binom_a = 1;    /* binom(m,0) = 1 */
            seq_t h = 1;          /* h_1 = 1 */
            for (idx_t k = 1; k <= m; ++k) {
                /* update binom to C(m,k) */
                binom_a = (binom_a *
                           (seq_t)(m - k + 1)) /
                          (seq_t)k;
                /* term = binom(m,k) * h_k * g_{m-k} */
                seq_t term = binom_a * h;
                term *= g_arr[m - k];
                sum_a += term;
                /* update h for next k: h_{k+1} = -k * h_k */
                h = -(seq_t)k * h;
            }
            a_arr[m] = sum_a;
        }
        computed_n = n;
    }

    return a_arr[n];
}

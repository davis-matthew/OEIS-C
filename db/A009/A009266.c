/*
 * A009266  Expansion of exp(tanh(x))/exp(x).
 * https://oeis.org/A009266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=1..n} G_k * a(n-k) * C(n-1, k-1), where G_1 = 0, G_2
 *            = 0, and for k ≥ 3, G_k = k! * coeff of x^k in tanh(x); a(0)=1
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The G_k are produced by the recurrence
   G_1 = 1, G_2 = 0, and for m ≥ 3
   G_m = -∑_{k=1}^{m-2} G_k * G_{m-1-k} * C(m-1, k) . */

#include <stddef.h>
#include <stdlib.h>

/* --- temporary storage for G_k coefficients --- */
static seq_t *G   = NULL;   /* G[k] -- see formula above */
static idx_t G_cap = 0;

/* Ensure G has capacity for indices 0..max_n inclusive */
static void ensure_G_capacity(idx_t max_n)
{
    if (G_cap <= max_n) {
        idx_t new_cap = max_n + 1;
        G = (seq_t *)realloc(G, new_cap * sizeof(seq_t));
        for (idx_t i = G_cap; i < new_cap; ++i) G[i] = (seq_t)0;
        G_cap = new_cap;
    }
}

static void build_G(idx_t max_n)
{
    if (max_n < 1) return;
    ensure_G_capacity(max_n);

    /* base values */
    G[1] = (seq_t)1;
    if (max_n >= 2) G[2] = (seq_t)0;

    for (idx_t m = 3; m <= max_n; ++m) {
        seq_t sum = (seq_t)0;
        /* binomial coefficient C(m-1, k) for current k */
        seq_t comb = (seq_t)(m - 1);   /* for k = 1 */
        for (idx_t k = 1; k <= m - 2; ++k) {
            sum += G[k] * G[m - 1 - k] * comb;
            if (k < m - 2) {
                comb = comb * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
            }
        }
        G[m] = -sum;
    }
}

/* --- temporary storage for a(n) sequence --- */
static seq_t *A   = NULL;   /* A[n] is the desired term */
static idx_t A_cap = 0;

/* Ensure A has capacity for indices 0..max_n inclusive */
static void ensure_A_capacity(idx_t max_n)
{
    if (A_cap <= max_n) {
        idx_t new_cap = max_n + 1;
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        for (idx_t i = A_cap; i < new_cap; ++i) A[i] = (seq_t)0;
        A_cap = new_cap;
    }
}

static void build_A(idx_t n)
{
    if (n < 0) return;
    ensure_A_capacity(n);

    /* a(0) = 1 */
    A[0] = (seq_t)1;

    /* We need G up to index n */
    build_G(n);

    for (idx_t i = 1; i <= n; ++i) {
        if (A[i] != 0) continue;   /* already computed */
        seq_t sum  = (seq_t)0;
        seq_t comb = (seq_t)1;        /* C(i-1, 0) for k=1 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t dk = (k == 1) ? (seq_t)0 : G[k];   /* d_k = G_k, but d_1 = 0 */
            if (dk != 0) {
                sum += dk * A[i - k] * comb;
            }
            if (k < i) {
                comb = comb * (seq_t)(i - k) / (seq_t)k;  /* next C(i-1, k) */
            }
        }
        A[i] = sum;
    }
}

seq_t a(idx_t n)
{
    build_A(n);
    return A[n];
}

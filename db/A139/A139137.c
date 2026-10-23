/*
 * A139137  Expansion of phi(q) / phi(q^3) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A139137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 32, i16 n <= 99, i32 n <= 326, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A139137
    A(n) is the coefficient of q^n in φ(q)/φ(q^3), where φ(q)=∑_{m∈ℤ}q^{m²}.
    The function computes the series by formal power‐series division:
    f(q)/g(q)=∑_{n≥0}h[n]q^n  with f(q)=φ(q) and g(q)=φ(q³).
    Since g[0]=1, h[0]=f[0]=1 and
        h[n] = f[n] - ∑_{k=1}^{n} g[k]*h[n-k] .
    g[k]≠0 only for k=3t² (t>0); f[n]≠0 only for n=t² (t>0).          */

#include <stdlib.h>

static seq_t *f = NULL;
static seq_t *g = NULL;
static seq_t *h = NULL;
static idx_t capacity = 0;          /* number of elements allocated          */
static idx_t computed = 0;          /* highest index computed + 1             */

static idx_t *g_idx = NULL;         /* indices where g[k]≠0                 */
static size_t g_idx_cap = 0;        /* allocated size for g_idx             */
static size_t g_idx_len = 0;        /* actual length of g_idx               */

/* ensure storage for at least N+1 elements */
static void ensure_capacity(idx_t N)
{
    if (capacity <= N) {
        idx_t newCap = capacity ? capacity * 2 : 128;
        if (newCap < N + 1) newCap = N + 1;

        seq_t *f_new = (seq_t*)malloc(newCap * sizeof(seq_t));
        seq_t *g_new = (seq_t*)malloc(newCap * sizeof(seq_t));
        seq_t *h_new = (seq_t*)malloc(newCap * sizeof(seq_t));

        for (idx_t i = 0; i < capacity; ++i) {
            f_new[i] = f[i];
            g_new[i] = g[i];
            h_new[i] = h[i];
        }

        free(f); free(g); free(h);
        f = f_new; g = g_new; h = h_new;
        capacity = newCap;
    }
}

/* recompute f and g coefficients up to index N and build list of non‑zero
   indices of g.  All other entries of f,g are cleared to 0.         */
static void recompute_fg(idx_t N)
{
    for (idx_t i = 0; i <= N; ++i) {
        f[i] = 0;
        g[i] = 0;
    }
    f[0] = 1;
    g[0] = 1;

    /* f[n] = 2 if n is a perfect square > 0 */
    for (idx_t t = 1; t * t <= N; ++t) {
        f[t * t] = (seq_t)2;
    }

    /* g[n] = 2 if n = 3*t^2 (>0); also build g_idx list */
    g_idx_len = 0;
    for (idx_t t = 1; 3 * t * t <= N; ++t) {
        idx_t pos = 3 * t * t;
        g[pos] = (seq_t)2;
        if (g_idx_len == g_idx_cap) {
            g_idx_cap = g_idx_cap ? g_idx_cap * 2 : 32;
            g_idx = (idx_t*)realloc(g_idx, g_idx_cap * sizeof(idx_t));
        }
        g_idx[g_idx_len++] = pos;
    }
}

/* compute h[n] for all n from 'computed' up to N inclusive    */
static void compute_h_up_to(idx_t N)
{
    ensure_capacity(N);
    recompute_fg(N);

    for (idx_t i = computed; i <= N; ++i) {
        seq_t sum = 0;
        for (size_t t = 0; t < g_idx_len && g_idx[t] <= i; ++t) {
            idx_t k = g_idx[t];
            sum += g[k] * h[i - k];
        }
        h[i] = f[i] - sum;
    }
    computed = N + 1;
}

/* user visible function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n + 1 > computed) {
        compute_h_up_to(n);
    } else if (n + 1 > capacity) {
        ensure_capacity(n);
    }
    return h[n];
}

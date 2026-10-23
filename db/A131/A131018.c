/*
 * A131018  Expansion of (q^-3) * psi(q) / psi(q^25) in powers of q where
 *          psi() is a Ramanujan theta function.
 * https://oeis.org/A131018
 *
 * Offset:    -3    (a(-3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in the expansion of (q^-3) * ψ(q) /
 *            ψ(q^25), where ψ(q) = Σ_{m≥0} q^{m(m+1)/2}. The implementation
 *            builds the series for ψ(q) and ψ(q^25), performs formal series
 *            division, and shifts the result by three powers of q. Only
 *            integer arithmetic is used; the sequence values ar...
 * Range:     i8 n <= 467, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Helper to test whether an integer n is a triangular number
   n = m(m+1)/2 for some non‑negative integer m. */
static inline bool is_triangular(idx_t n)
{
    /* Solve m^2 + m - 2n = 0  ->  m = (-1 + sqrt(1+8n))/2  */
    double d = sqrt((double)(8LL * n + 1));
    idx_t s = (idx_t)(d + 0.5);          /* round to nearest integer   */
    if (s * s != 8LL * n + 1) return false;
    return ((s - 1) & 1) == 0;
}

/* Extension structure for the quotient series g = ψ(q) / ψ(q^25). */
static seq_t *g_series   = NULL;   /* series coefficients up to current_max */
static idx_t current_max = 0;      /* highest index computed (>=0) */

/* Store the exponents k for which ψ(q^25) has coefficient 1, i.e.
   k = 25 * T(m) where T(m) = m(m+1)/2.  These are needed
   for the convolution in the division recurrence. */
static idx_t *d_indices   = NULL;
static size_t d_count     = 0;     /* number of stored indices   */
static size_t d_capacity  = 0;
static idx_t d_next_m     = 1;     /* next m to generate */

/* Ensure the array d_indices contains all indices ≤ limit. */
static void ensure_d_indices(idx_t limit)
{
    while (true) {
        idx_t k = 25ULL * d_next_m * (d_next_m + 1) / 2ULL;
        if (k > limit) break;
        if (d_count >= d_capacity) {
            /* grow storage (not needed for n≈10000, but safe) */
            size_t new_cap = (d_capacity == 0) ? 16 : d_capacity * 2;
            d_indices = (idx_t*)realloc(d_indices, new_cap * sizeof(idx_t));
            d_capacity = new_cap;
        }
        d_indices[d_count++] = k;
        ++d_next_m;
    }
}

/* Extend the series g_series up to index 'target' (inclusive). */
static void extend_series(idx_t target)
{
    if (target <= current_max) return;        /* already computed */

    size_t new_size = target + 1;
    g_series = (seq_t*)realloc(g_series, new_size * sizeof(seq_t));

    /* Ensure the d_indices needed for all new positions are ready. */
    ensure_d_indices(target);

    /* Compute coefficients from current_max + 1 up to target. */
    for (idx_t i = current_max + 1; i <= target; ++i) {
        int c = is_triangular(i) ? 1 : 0;          /* coefficient of ψ(q) */
        seq_t sum = 0;                             /* Σ d[k] * g[i-k] , d[k]=1 */
        for (size_t t = 0; t < d_count; ++t) {
            idx_t k = d_indices[t];
            if (k > i) break;                      /* d_indices sorted */
            sum += g_series[i - k];
        }
        g_series[i] = (seq_t)c - sum;              /* recurrence */
    }
    current_max = target;
}

/* External function required by the caller. */
seq_t a(idx_t n)
{
    if (n < -3) return 0;          /* values below the offset are irrelevant */

    /* Shift from the definition: a(n) is the coefficient of q^{n+3}. */
    idx_t target = n + 3;

    /* Lazy initialization of the series. */
    if (g_series == NULL) {
        g_series = (seq_t*)malloc(1 * sizeof(seq_t));
        g_series[0] = 1;            /* ψ(q)/ψ(q^25) starts with 1 */
        current_max = 0;
    }

    extend_series(target);
    return g_series[target];
}

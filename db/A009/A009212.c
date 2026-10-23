/*
 * A009212  E.g.f. exp(sin(x)*exp(x)).
 * https://oeis.org/A009212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A009212: E.g.f. exp(sin(x)*exp(x))
   a(0) = 1
   For n ≥ 1:
   a[n] = Σ_{k=1..n} g[k] * a[n-k] * C(n-1, k-1)
   where g[k] = Σ_{t odd ≤ k} (-1)^{(t-1)/2} * C(k, t)   (integer)
   The computation uses a static memoised cache so that calls can come in any order.  */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_cache = NULL;   /* a[n] values */
static seq_t *g_cache = NULL;   /* g[n] values  */
static size_t a_cap = 0;        /* current capacity of a_cache (size) */
static size_t g_cap = 0;        /* current capacity of g_cache */

/* Ensure the caches contain values up to index max_n (inclusive). */
static void ensure_capacity(idx_t max_n)
{
    if (max_n < a_cap) return; /* already computed */

    size_t new_cap = (size_t)max_n + 1;
    seq_t *new_a = NULL;
    seq_t *new_g = NULL;

    /* Allocate new arrays */
    new_a = (seq_t *)malloc(new_cap * sizeof(seq_t));
    new_g = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_a || !new_g) return; /* allocation failed */

    /* Copy old data */
    for (size_t i = 0; i < a_cap; ++i) new_a[i] = a_cache[i];
    for (size_t i = 0; i < g_cap; ++i) new_g[i] = g_cache[i];

    /* Initialise new entries */
    /* a[0] is defined as 1 */
    if (new_cap > 0) new_a[0] = (seq_t)1;
    /* Compute g[k] for k ≥ old_g_cap */
    for (size_t k = g_cap; k < new_cap; ++k) {
        seq_t bin = (seq_t)1;  /* C(k,0) */
        seq_t gk = (seq_t)0;
        for (size_t t = 1; t <= k; ++t) {
            bin = bin * (seq_t)(k - t + 1) / (seq_t)t;  /* C(k,t) */
            if ((t & 1U) == 0U) continue;              /* skip even t */
            size_t half = (t - 1U) >> 1U;              /* (t-1)/2 */
            if ((half & 1U) != 0U)   gk -= bin;         /* negative sign */
            else                     gk += bin;         /* positive sign */
        }
        new_g[k] = gk;
    }

    /* Compute a[n] for n ≥ old_a_cap */
    for (size_t n = a_cap; n < new_cap; ++n) {
        if (n == 0) continue;  /* already set */
        seq_t total = (seq_t)0;
        seq_t B = (seq_t)1;    /* C(n-1,0) */
        for (size_t k = 1; k <= n; ++k) {
            seq_t term = new_g[k] * new_a[n - k] * B;
            total += term;
            if (k < n) {
                /* Update B to C(n-1,k) */
                B = (B * (seq_t)(n - k)) / (seq_t)k;
            }
        }
        new_a[n] = total;
    }

    /* Free old arrays */
    free(a_cache);
    free(g_cache);
    a_cache = new_a;
    g_cache = new_g;
    a_cap = new_cap;
    g_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure we have computed up to n */
    ensure_capacity(n);
    return a_cache[(size_t)n];
}

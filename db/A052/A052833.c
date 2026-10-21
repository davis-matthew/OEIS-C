/*
 * A052833  Expansion of e.g.f.: (log(2-exp(x)))^2.
 * https://oeis.org/A052833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   define h[0] = 0, h[1] = 1. For n ≥ 2 h[n] = 2 + Σ_{t=1}^{n-2}
 *            C(n-1,t) * h[t+1] then a[n] = Σ_{k=1}^{n-1} C(n,k) * h[k] *
 *            h[n-k]
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *h_vals = NULL;          /* h[n] values (stored as absolute values) */
static seq_t *a_vals = NULL;          /* a[n] values */
static size_t h_cap   = 0;
static size_t a_cap   = 0;
static idx_t  cache_n = 0;            /* largest n that has been computed */

/* Ensure array allocated with at least 'need' elements */
static void ensure_cap(size_t *cap_ptr, seq_t **arr_ptr, size_t need)
{
    if (*cap_ptr >= need) return;
    size_t new_cap = (*cap_ptr == 0) ? 1 : *cap_ptr << 1;
    while (new_cap < need) new_cap <<= 1;
    *arr_ptr = (seq_t *)realloc(*arr_ptr, new_cap * sizeof(seq_t));
    *cap_ptr = new_cap;
}

seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------
       Lazily compute and cache values up to n.  This works for any n
       and does not rely on any external library besides the big‑int
       support that provides seq_t arithmetic.
       -------------------------------------------------------------------- */
    if (n == 0) return (seq_t)0;           /* a(0) = 0 */
    if (n == 1) return (seq_t)0;           /* a(1) = 0 */

    /* Ensure we have cached up to n */
    if (cache_n < n) {
        /* Allocate or grow the storage */
        ensure_cap(&h_cap, &h_vals, (size_t)n + 1);
        ensure_cap(&a_cap, &a_vals, (size_t)n + 1);

        /* Initialize first few entries if they are not already set */
        if (cache_n == 0) {
            h_vals[0] = (seq_t)0;   /* h[0] = 0 */
            a_vals[0] = (seq_t)0;   /* a[0] = 0 */
            h_vals[1] = (seq_t)1;   /* h[1] = 1 */
            a_vals[1] = (seq_t)0;   /* a[1] = 0 */
            cache_n   = 1;
        }

        /* Compute values from cache_n+1 up to n */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            if (i == 0) { h_vals[0] = (seq_t)0; a_vals[0] = (seq_t)0; continue; }
            if (i == 1) { h_vals[1] = (seq_t)1; a_vals[1] = (seq_t)0; continue; }

            /* ---- Compute h[i] ---- */
            seq_t sum  = (seq_t)2;                 /* start with the constant 2 */
            seq_t cur  = (seq_t)(i - 1);           /* binom(i-1, 1) */

            /* Run through t = 1 .. i-2 (corresponding to binom(i-1,t)) */
            for (idx_t t = 1; t <= i-2; ++t) {
                seq_t term = cur * h_vals[(size_t)(t + 1)];
                sum += term;

                /* Update cur to binom(i-1, t+1) */
                seq_t numer = cur * (seq_t)((i - 1) - t);
                cur = numer / (seq_t)(t + 1);
            }
            h_vals[(size_t)i] = sum;

            /* ---- Compute a[i] ---- */
            seq_t sum_a = (seq_t)0;
            seq_t cur_a = (seq_t)i;                    /* binom(i,1) */

            for (idx_t k = 1; k <= i-1; ++k) {
                seq_t term = cur_a * h_vals[(size_t)k];
                term *= h_vals[(size_t)(i - k)];
                sum_a += term;

                /* Update cur_a to binom(i, k+1) */
                seq_t numer_a = cur_a * (seq_t)(i - k);
                cur_a = numer_a / (seq_t)(k + 1);
            }
            a_vals[(size_t)i] = sum_a;
        }
        cache_n = n;
    }

    return a_vals[(size_t)n];
}

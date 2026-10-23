/*
 * A141154  L.g.f.: A(x) = log( 1 + Sum_{n>=1} (n-1)!*x^n ) = Sum_{n>=1}
 *          a(n)*x^n/n.
 * https://oeis.org/A141154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is defined by
 *  A(x) = log(1 + Σ_{k≥1} (k-1)!·x^k ),
 *  so that exp(A(x)) = 1 + x + x^2 + 2x^3 + 6x^4 + 24x^5 + … .
 *  Let B(x) = 1 + Σ_{k≥1} (k-1)!·x^k  and  G(x) = B'(x)/B(x) = Σ_{n≥0} g_n x^n .
 *  Then a(n) = g_{n-1} for n≥1.  The coefficients g_n satisfy
 *     g_0 = 1!,
 *     g_n = (n+1)! – Σ_{k=0}^{n-1} g_k · (n-k-1)!   (n≥1) .
 *  The implementation builds the factorials, the coefficients (n‑1)!, and
 *  the g_n values incrementally, caching all results for future calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Assumes n ≥ 1; undefined for n < 1. */

    /* Factorials: fact[i] = i!   (fact[0] = 0! = 1) */
    static seq_t *fact_arr = NULL;
    static idx_t fact_len = 0;

    /* B coefficients: B[i] = (i-1)! for i≥1, B[0] = 1 */
    static seq_t *B_arr = NULL;
    static idx_t B_len = 0;

    /* g coefficients: g[n] = coefficient of x^n in G(x) = B'(x)/B(x) */
    static seq_t *g_arr = NULL;
    static idx_t g_len = 0;

    /* a values: a[n] = required sequence value (a[1] = g[0]) */
    static seq_t *a_arr = NULL;
    static idx_t a_len = 0;
    static idx_t max_computed = 0;

    /* Return cached value if available */
    if (n <= max_computed)
        return a_arr[n];

    /* Compute up to the largest required index (≥ n+1) */
    idx_t needed = n + 1;            /* size needed for A terms (index 0 unused) */

    /* --------------------------------------------------------------------
     *  Build factorials up to needed+1 (so fact[needed+1] is available)
     * -------------------------------------------------------------------- */
    if (fact_len == 0) {
        fact_len = 1;
        fact_arr = (seq_t*)malloc(fact_len * sizeof(seq_t));
        if (!fact_arr) abort();
        fact_arr[0] = 1;           /* 0! = 1 */
    }
    if (fact_len < needed + 1) {
        size_t newlen = (size_t)(needed + 1);
        seq_t *tmp = (seq_t*)realloc(fact_arr, newlen * sizeof(seq_t));
        if (!tmp) abort();
        fact_arr = tmp;
        for (idx_t i = fact_len; i < newlen; ++i)
            fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
        fact_len = newlen;
    }

    /* --------------------------------------------------------------------
     *  Build B coefficients: B[0] = 1, B[i] = (i-1)!  for i≥1
     * -------------------------------------------------------------------- */
    if (B_len < needed) {
        size_t newlen = (size_t)needed;
        seq_t *tmp = (seq_t*)realloc(B_arr, newlen * sizeof(seq_t));
        if (!tmp) abort();
        B_arr = tmp;
        if (B_len == 0)
            B_arr[0] = 1;          /* B[0] = 1 */
        for (idx_t i = 1; i < newlen; ++i)
            B_arr[i] = fact_arr[i - 1];   /* (i-1)! */
        B_len = newlen;
    }

    /* --------------------------------------------------------------------
     *  Compute g coefficients up to index n-1
     * -------------------------------------------------------------------- */
    if (g_len < n) {
        size_t newlen = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(g_arr, newlen * sizeof(seq_t));
        if (!tmp) abort();
        g_arr = tmp;
        for (idx_t i = g_len; i < n; ++i) {
            seq_t Bprime = fact_arr[i + 1];   /* (i+1)! = derivative coefficient */
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k)
                sum += g_arr[k] * B_arr[i - k];
            g_arr[i] = Bprime - sum;
        }
        g_len = newlen;
    }

    /* --------------------------------------------------------------------
     *  Build a values (a[1] = g[0])
     * -------------------------------------------------------------------- */
    if (a_len < needed) {
        size_t newlen = (size_t)needed;
        seq_t *tmp = (seq_t*)realloc(a_arr, newlen * sizeof(seq_t));
        if (!tmp) abort();
        a_arr = tmp;
        for (idx_t i = a_len; i < newlen; ++i) {
            if (i == 0) continue;          /* a(0) is unused */
            a_arr[i] = g_arr[i - 1];
        }
        a_len = newlen;
    }

    max_computed = n;
    return a_arr[n];
}

/*
 * A217138  G.f. A(x) satisfies A(x) = 1 + x*A(x)^2*(A(x) + A(-x))/2.
 * https://oeis.org/A217138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217138.
   Formula: a(0)=1; for n>0
   a(n) = Sum_{i,j,k≥0, i+j+2k=n-1} a(i)*a(j)*a(2k)          */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* calloc, realloc, abort, memset */

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------
       Static cache for previously computed terms.
       a_cache[m] = a(m);  a_len = number of terms stored.
       ------------------------------------------------------------ */
    static seq_t *a_cache = NULL;
    static idx_t  a_len   = 0;
    static idx_t  a_cap   = 0;

    /* ------------------------------------------------------------
       Static cache for convolution of the sequence with itself.
       conv_cache[m] = Σ_{i=0..m} a(i)*a(m-i)   (ordered pairs)
       ------------------------------------------------------------ */
    static seq_t *conv_cache = NULL;
    static idx_t  conv_cap   = 0;

    /* ------------------------------------------------------------
       Lazy initialisation.
       ------------------------------------------------------------ */
    if (a_cap == 0) {
        a_cap   = 1;
        a_cache = (seq_t*)calloc(a_cap, sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = 1;          /* a(0) = 1 */
        a_len    = 1;

        conv_cap   = 1;
        conv_cache = (seq_t*)calloc(conv_cap, sizeof(seq_t));
        if (!conv_cache) abort();
        conv_cache[0] = 1;       /* conv[0] = a0*a0 = 1 */
    }

    /* ------------------------------------------------------------
       Fast path for the first term.
       ------------------------------------------------------------ */
    if (n == 0) return a_cache[0];

    /* ------------------------------------------------------------
       Ensure a_cache is large enough.
       ------------------------------------------------------------ */
    if (a_cap <= n) {
        size_t old_cap = (size_t)a_cap;
        size_t new_cap = (size_t)(n + 1) * 2;  /* overshoot to reduce reallocs */
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        if (new_cap > old_cap)
            memset(a_cache + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
        a_cap = (idx_t)new_cap;
    }

    /* ------------------------------------------------------------
       Ensure conv_cache is large enough (indices up to 2*n).
       ------------------------------------------------------------ */
    if (conv_cap <= 2 * n) {
        size_t old_cap = (size_t)conv_cap;
        size_t new_cap = (size_t)(2 * n + 1) * 2;  /* overshoot */
        seq_t *tmp = (seq_t*)realloc(conv_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        conv_cache = tmp;
        if (new_cap > old_cap)
            memset(conv_cache + old_cap, 0,
                   (new_cap - old_cap) * sizeof(seq_t));
        conv_cap = (idx_t)new_cap;
    }

    /* ------------------------------------------------------------
       Compute missing terms up to n.
       ------------------------------------------------------------ */
    for (idx_t m = a_len; m <= n; ++m) {
        /* Sum over k = 0 .. floor((m-1)/2) */
        seq_t sum = 0;
        idx_t maxK = (m > 0) ? (m - 1) / 2 : 0;
        for (idx_t k = 0; k <= maxK; ++k) {
            idx_t rem = (m - 1) - 2 * k;            /* 0 <= rem <= m-1 */
            seq_t term = conv_cache[rem] * a_cache[2 * k];
            sum += term;
        }
        a_cache[m] = sum;

        /* Update convolution cache with the new term a[m] */
        seq_t a_m = a_cache[m];
        for (idx_t i = 0; i < m; ++i) {
            seq_t prod = a_m * a_cache[i];
            conv_cache[m + i] += prod;   /* pair (m,i) */
            conv_cache[m + i] += prod;   /* pair (i,m) */
        }
        conv_cache[2 * m] += a_m * a_m;  /* pair (m,m) counted once */
    }

    a_len = n + 1;
    return a_cache[n];
}

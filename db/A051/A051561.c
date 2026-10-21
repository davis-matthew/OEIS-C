/*
 * A051561  Third unsigned column of triangle A051379.
 * https://oeis.org/A051561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n = 0,1 a(n) = ∑_{k=0}^{n-2} C(n,k) * c_{n-k} *
 *            risingfac(8,k), with c_0 = c_1 = 0, c_m = (m-1) * c_{m-1} +
 *            (m-2)! for m ≥ 2, risingfac(8,k) = ∏_{j=0}^{k-1} (8 + j) (by
 *            definition, risingfac(8,0)=1). The sequence is independent of
 *            the sign of the signed Stirling numbers of the first...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *c_cache = NULL;   /* c_m values          */
static seq_t *b_cache = NULL;   /* risingfac(8,k)      */
static seq_t *fact_cache = NULL; /* factorials          */
static seq_t *a_cache = NULL;   /* a(n) values         */
static idx_t n_cap = 0;         /* current cache size  */

static void ensure_capacity(idx_t n)
{
    if (n+1 <= n_cap) return;                /* already enough */

    idx_t new_cap = n + 1;
    /* reallocate arrays */
    c_cache   = (seq_t*)realloc(c_cache,   new_cap * sizeof(seq_t));
    b_cache   = (seq_t*)realloc(b_cache,   new_cap * sizeof(seq_t));
    fact_cache= (seq_t*)realloc(fact_cache, new_cap * sizeof(seq_t));
    a_cache   = (seq_t*)realloc(a_cache,   new_cap * sizeof(seq_t));

    /* initialize new entries */
    for (idx_t i = n_cap; i < new_cap; ++i) {
        if (i == 0) { c_cache[i] = 0; b_cache[i] = 1; fact_cache[i] = 1; a_cache[i] = 0; }
        else if (i == 1) { c_cache[i] = 0; b_cache[i] = 1; fact_cache[i] = (seq_t)1; a_cache[i] = 0; }
        else { /* factorial */
            fact_cache[i] = fact_cache[i-1] * (seq_t)(i);
        }
    }

    /* compute c[2..] using recurrence c_m = (m-1)*c_{m-1} + (m-2)! */
    for (idx_t m = 2; m < new_cap; ++m) {
        c_cache[m] = (seq_t)(m-1) * c_cache[m-1] + fact_cache[m-2];
    }

    /* compute risingfac(8,k) */
    for (idx_t k = 1; k < new_cap; ++k) {
        b_cache[k] = b_cache[k-1] * (seq_t)(7 + k);   /* 8 + (k-1) = 7 + k */
    }

    /* compute a values for new indices */
    for (idx_t i = n_cap; i < new_cap; ++i) {
        if (i < 2) { a_cache[i] = 0; continue; }   /* defined by sequence */
        seq_t sum = 0;
        seq_t bin = 1;                              /* C(i,0) */
        for (idx_t k = 0; k <= i - 2; ++k) {
            /* term = binom(i,k) * c_{i-k} * risingfac(8,k) */
            seq_t term = bin;
            term *= c_cache[i - k];
            term *= b_cache[k];
            sum += term;

            if (k < i - 2) {
                bin *= (seq_t)(i - k);
                bin /= (seq_t)(k + 1);
            }
        }
        a_cache[i] = sum;
    }

    n_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return a_cache[n];
}

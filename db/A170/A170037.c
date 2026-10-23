/*
 * A170037  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1 and for n≥1 a(n) = rhs(n) + 26·Σ_{k=1}^{35} a(n‑k) –
 *            351·a(n‑36), where rhs(0)=1, rhs(1 … 35)=2, rhs(36)=1, otherwise
 *            0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed terms in a dynamically
   resizable static array.  It works for any seq_t (fixed or
   arbitrary‑precision) and any idx_t (unsigned 64‑bit).  No external
   utilities are required. */

#include <stdlib.h>   /* malloc, realloc */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
/* ------------------------------------------------------------------ */
{
    /* static cache of computed terms a(0)…a(cache_len‑1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* allocated capacity          */
    static size_t cache_len = 0;  /* number of terms stored      */

    /* initialise cache if first call */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;   /* allocation failure               */
        cache[0] = (seq_t)1;          /* a(0) = 1                          */
        cache_len = 1;
    }

    /* enlarge cache to accommodate index n if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            /* double capacity to reduce realloc frequency */
            size_t dbl = cache_cap * 2;
            if (dbl < new_cap) dbl = new_cap;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * dbl);
            if (!tmp) return (seq_t)0;   /* allocation failure               */
            cache = tmp;
            cache_cap = dbl;
        }

        /* compute terms from current cache_len up to n (inclusive) */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* rhs(i) */
            seq_t rhs;
            if (i == 0)
                rhs = (seq_t)1;
            else if ((idx_t)i <= 35)
                rhs = (seq_t)2;
            else if ((idx_t)i == 36)
                rhs = (seq_t)1;
            else
                rhs = (seq_t)0;

            /* Σ_{k=1}^{35} a(i‑k) with i‑k ≥ 0 */
            seq_t sum35 = (seq_t)0;
            for (idx_t k = 1; k <= 35 && i >= k; ++k)
                sum35 += cache[i - k];

            /* a(i) = rhs + 26*sum35 – 351*a(i‑36) (if i ≥ 36) */
            seq_t term = rhs + (seq_t)26 * sum35;
            if (i >= 36)
                term -= (seq_t)351 * cache[i - 36];

            cache[i] = term;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}

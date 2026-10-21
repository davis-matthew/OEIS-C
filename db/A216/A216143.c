/*
 * A216143  E.g.f.: exp( Shw(x) ) where Shw(x) = Sum_{n>=0} (2*n+2)^(2*n) *
 *          x^(2*n+1)/(2*n+1)!.
 * https://oeis.org/A216143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n≥1 a(n) = Σ_{k=1}^{n} C(n-1,k-1) * b(k) * a(n-k),
 *            where b(k) = (k+1)^(k-1) if k is odd, otherwise 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static caches for a(n) and b(k) */
    static seq_t *a_cache   = NULL;
    static idx_t  a_cap     = 0;
    static idx_t  a_max     = 0;

    static seq_t *b_cache   = NULL;
    static idx_t  b_cap     = 0;
    static idx_t  b_max     = 0;

    /* first call initialisation */
    if (a_cache == NULL) {
        a_cap   = 1;
        a_cache = (seq_t*)calloc(a_cap, sizeof(seq_t));
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        a_max   = 0;

        b_cap   = 1;
        b_cache = (seq_t*)calloc(b_cap, sizeof(seq_t));
        b_cache[0] = (seq_t)0;   /* unused except index 0 */
        b_max   = 0;
    }

    /* grow a_cache if necessary */
    if (n > a_cap - 1) {
        idx_t new_cap = a_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        for (idx_t i = a_cap; i < new_cap; ++i) a_cache[i] = (seq_t)0;
        a_cap = new_cap;
    }

    /* grow b_cache if necessary */
    if (n > b_cap - 1) {
        idx_t new_cap = b_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        b_cache = (seq_t*)realloc(b_cache, new_cap * sizeof(seq_t));
        for (idx_t i = b_cap; i < new_cap; ++i) b_cache[i] = (seq_t)0;
        b_cap = new_cap;
    }

    /* pre‑compute b(k) for odd k up to n */
    if (b_max < n) {
        for (idx_t k = b_max + 1; k <= n; ++k) {
            if (k & 1U) /* odd */
                b_cache[k] = big_pow((seq_t)(k + 1), (seq_t)(k - 1));
            else
                b_cache[k] = (seq_t)0;
        }
        b_max = n;
    }

    /* compute a values up to n */
    for (idx_t t = a_max + 1; t <= n; ++t) {
        seq_t sum = (seq_t)0;
        seq_t c   = (seq_t)1;  /* C(t-1,0) */

        for (idx_t k = 1; k <= t; ++k) {
            if (k & 1U) { /* k odd, b(k) non‑zero */
                seq_t term = a_cache[t - k];
                term *= b_cache[k];
                term *= c;
                sum += term;
            }
            if (k < t) {   /* update binomial C(t-1,k) for next k+1 */
                c = c * (seq_t)(t - k);
                c = c / (seq_t)k;   /* exact division */
            }
        }
        a_cache[t] = sum;
    }
    a_max = n;
    return a_cache[n];
}

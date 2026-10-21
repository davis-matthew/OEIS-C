/*
 * A060905  Expansion of e.g.f. exp(x*exp(x) + 1/2*x^2*exp(x)^2).
 * https://oeis.org/A060905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} binomial(n-1, k-1) · c_k · a(n-k) * where *
 *            c_1 = 1 * c_k = k + binomial(k,2) · 2^(k-2) (k ≥ 2) * * This is
 *            the standard recurrence for the number of functions f:[n]->[n] *
 *            satisfying f³(x)=f(x). All arithmetic uses seq_t, so the *
 *            implementation works for any unsigned integer...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Memoised cache for a(n)                                            */
/* ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;
static idx_t  a_cap   = 0;

/* ------------------------------------------------------------------ */
/*  Extend a_cache to hold a(n) for n = 0 .. need                      */
/* ------------------------------------------------------------------ */
static void extend_a(idx_t need)
{
    if (need + 1 <= a_cap)
        return;

    seq_t *newv = (seq_t*)realloc(a_cache, (need + 1) * sizeof(seq_t));
    a_cache = newv;

    for (idx_t n = a_cap; n <= need; ++n) {
        if (n == 0) {
            a_cache[0] = (seq_t)1;
        } else {
            seq_t sum  = (seq_t)0;
            seq_t comb = (seq_t)1;      /* binomial(n-1,0) */
            seq_t pow2 = (seq_t)1;      /* 2^(k-2), start with k=2 */

            for (idx_t k = 1; k <= n; ++k) {
                /* compute c_k */
                seq_t c_k;
                if (k == 1) {
                    c_k = (seq_t)1;
                } else {
                    seq_t term2 = ((seq_t)k * (seq_t)(k - 1)) / (seq_t)2; /* C(k,2) */
                    term2 = term2 * pow2;          /* * 2^(k-2) */
                    c_k = (seq_t)k + term2;
                }

                seq_t term = comb * c_k;
                term = term * a_cache[n - k];
                sum  = sum + term;

                /* update comb for next k: C(n-1,k) */
                if (k < n) {
                    comb = comb * (seq_t)(n - k);
                    comb = comb / (seq_t)k;   /* exact division */
                }

                /* advance pow2 for next k (k >= 2) */
                if (k >= 2)
                    pow2 = pow2 * (seq_t)2;
            }
            a_cache[n] = sum;
        }
    }
    a_cap = need + 1;
}

/* ------------------------------------------------------------------ */
/*  Public function defined as required                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    extend_a(n);
    return a_cache[n];
}

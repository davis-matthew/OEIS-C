/*
 * A200320  E.g.f. satisfies: A(x) = x-1 + exp(A(x)^2/2).
 * https://oeis.org/A200320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A200320 - sequence defined by * A(x) = x-1 +
 *            exp(A(x)^2/2) * with a(1)=1. * Recurrence derived from the
 *            compositional reversion: * w_k = sum_{i=1}^{k-1}
 *            C(k,i)*a_i*a_{k-i} * u_k = w_k/2 * S_k = sum_{i=0}^{k-1}
 *            C(k-1,i)*S_i*u_{k-i} * a_k = (k==1?1:0)+S_k (a_1 is fixed to 1)
 *            * All computatio...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* realloc, abort */

static idx_t  cache_n  = 0;          /* largest n that has been computed   */
static seq_t *a_cache = NULL;        /* a values                           */
static seq_t *S_cache = NULL;        /* S values                           */
static seq_t *u_cache = NULL;        /* u values                           */

static void ensure_cache(idx_t n)
{
    if (n <= cache_n) return;               /* already known */

    /* enlarge all arrays to hold indices 0…n */
    size_t new_len = (size_t)n + 1;         /* new length including 0 */
    a_cache = (seq_t*)realloc(a_cache, new_len * sizeof(seq_t));
    S_cache = (seq_t*)realloc(S_cache, new_len * sizeof(seq_t));
    u_cache = (seq_t*)realloc(u_cache, new_len * sizeof(seq_t));
    if (!a_cache || !S_cache || !u_cache) abort();  /* out of memory */

    /* base values (repeating them avoids uninitialised reads) */
    a_cache[0] = (seq_t)0;
    S_cache[0] = (seq_t)1;
    u_cache[0] = (seq_t)0;

    /* compute new terms */
    for (idx_t k = cache_n + 1; k <= n; ++k) {
        /* w_k = sum C(k,i)*a_i*a_{k-i}   (i=1..k-1) */
        seq_t w = 0;
        seq_t binom = 1;                 /* C(k,0) = 1  */
        for (idx_t i = 1; i < k; ++i) {
            binom = binom * (seq_t)(k - (i - 1)); /* C(k,i) from C(k,i-1) */
            binom /= (seq_t)i;                 /* exact division */
            seq_t prod = a_cache[i] * a_cache[k - i];
            prod = prod * binom;
            w += prod;
        }
        u_cache[k] = w / 2;               /* w is always even */

        /* S_k = sum C(k-1,i)*S_i*u_{k-i}   (i=0..k-1) */
        seq_t sumS = 0;
        binom = 1;                         /* C(k-1,0) */
        for (idx_t i = 0; i < k; ++i) {
            seq_t term = S_cache[i] * u_cache[k - i];
            term = term * binom;
            sumS += term;
            if (i + 1 < k) {
                binom = binom * (seq_t)(k - 1 - i); /* next binomial */
                binom /= (seq_t)(i + 1);
            }
        }
        S_cache[k] = sumS;

        /* a_k = S_k (plus 1 for k=1) */
        seq_t add = (k == (idx_t)1) ? (seq_t)1 : (seq_t)0;
        a_cache[k] = add + S_cache[k];
    }
    cache_n = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}

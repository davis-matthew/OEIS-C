/*
 * A215128  G.f.: Sum_{n>=0} d^n/dx^n (x + x^2)^(2*n) / n!.
 * https://oeis.org/A215128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * A214372(n+1) where G(x)=x+(G+G^2)^2 and
 *            A214372(k) is the coefficient of x^k in G(x)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static arrays to cache the coefficients of G(x), pair sums and triple sums */
static seq_t *g     = NULL;   /* g[k] = coefficient of x^k in G(x)   */
static seq_t *S2    = NULL;   /* S2[k] = sum_{i=1}^{k-1} g[i]*g[k-i]  */
static seq_t *T3    = NULL;   /* T3[k] = sum_{i+j+k=k} g[i]g[j]g[k]   */
static idx_t   g_sz = 0;       /* size of the allocated arrays          */

/* Ensure that the arrays are size at least (up_to+1) */
static int ensure_size(idx_t up_to)
{
    if (g_sz > up_to) return 0;                 /* already enough */

    idx_t old = g_sz;
    idx_t new_sz = up_to + 1;

    seq_t *new_g  = (seq_t *)realloc(g,  new_sz * sizeof(seq_t));
    seq_t *new_S2 = (seq_t *)realloc(S2, new_sz * sizeof(seq_t));
    seq_t *new_T3 = (seq_t *)realloc(T3, new_sz * sizeof(seq_t));
    if (!new_g || !new_S2 || !new_T3) return -1;

    g  = new_g;
    S2 = new_S2;
    T3 = new_T3;

    /* initialise new elements */
    for (idx_t i = old; i < new_sz; ++i) {
        g[i]  = 0;
        S2[i] = 0;
        T3[i] = 0;
    }

    /* compute the newly needed coefficients */
    for (idx_t n = old; n < new_sz; ++n) {
        if (n == 0) {                            /* g[0] = 0 by definition */
            g[0]  = 0;
            S2[0] = 0;
            T3[0] = 0;
            continue;
        }
        if (n == 1) {                            /* g[1] = 1 from the equation */
            g[1]  = 1;
            S2[1] = 0;
            T3[1] = 0;
            continue;
        }

        /* pair sum: S2[n] = Σ g[i]*g[n-i]   (i=1..n-1) */
        seq_t pairs = 0;
        for (idx_t i = 1; i <= n-1; ++i) {
            pairs += g[i] * g[n-i];
        }
        S2[n] = pairs;

        seq_t val = pairs;

        /* triple sum (without factor 2): T3[n] = Σ g[k]*S2[n-k]  (k=1..n-1) */
        seq_t triple = 0;
        for (idx_t k = 1; k <= n-1; ++k) {
            triple += g[k] * S2[n-k];
        }
        val += 2 * triple;
        T3[n] = triple;

        /* quadruple sum: Σ g[k]*T3[n-k]   (k=1..n-1) */
        seq_t quadruple = 0;
        for (idx_t k = 1; k <= n-1; ++k) {
            quadruple += g[k] * T3[n-k];
        }
        val += quadruple;

        g[n] = val;
    }

    g_sz = new_sz;
    return 0;
}

/* --- public function required by the problem statement --- */
seq_t a(idx_t n)
{
    idx_t target = n + 1;                     /* need G coefficient of x^{n+1} */
    if (ensure_size(target) != 0) return 0;   /* on failure return 0 */

    seq_t result = g[target] * ((seq_t)n + 1);
    return result;
}

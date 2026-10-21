/*
 * A200074  G.f. satisfies A(x) = (1 + x*A(x)^2)*(1 + x^2*A(x)).
 * https://oeis.org/A200074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200074:  coefficients of the power series A(x) satisfying
   A(x) = (1 + x*A(x)^2)*(1 + x^2*A(x)).
   Recurrence (derived from the g.f.):  a(0)=1;
   for n≥1:
      a(n) = v(n‑1) + a(n‑2) + w(n‑3)   (terms omitted when indices <0),
   where
      v(k) = Σ_{i=0}^k a(i)*a(k‑i)      -- coefficient of A(x)^2
      w(k) = Σ_{i=0}^k a(i)*v(k‑i)      -- coefficient of A(x)^3
   The function memoizes computed values and works for any order of calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for a, v, w sequences */
    static seq_t *A         = NULL;
    static seq_t *V         = NULL;
    static seq_t *W         = NULL;
    static size_t  cap      = 0;          /* current allocation size   */
    static idx_t  last     = (idx_t)-1;   /* highest index COMPUTED    */

    /* Ensure we have space for indices up to n */
    size_t need = n + 1;
    if (cap < need) {
#ifdef __cplusplus
        seq_t *newA = new seq_t[need];
        seq_t *newV = new seq_t[need];
        seq_t *newW = new seq_t[need];
        if (cap > 0) {
            for (size_t i = 0; i < cap; ++i) {
                newA[i] = A[i];
                newV[i] = V[i];
                newW[i] = W[i];
            }
            delete[] A;
            delete[] V;
            delete[] W;
        }
        A = newA;
        V = newV;
        W = newW;
#else
        A = (seq_t*)realloc(A, need * sizeof(seq_t));
        V = (seq_t*)realloc(V, need * sizeof(seq_t));
        W = (seq_t*)realloc(W, need * sizeof(seq_t));
#endif
        cap = need;
    }

    /* If nothing has been computed yet, initialise the base case */
    if (last == (idx_t)-1) {
        A[0] = (seq_t)1;
        V[0] = (seq_t)1;        /* a(0)^2  */
        W[0] = (seq_t)1;        /* a(0)^3  */
        last = 0;
    }

    /* Compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = V(i-1) + (i>=2? a(i-2):0) + (i>=3? W(i-3):0) */
        seq_t term = V[i-1];
        if (i >= 2) term += A[i-2];
        if (i >= 3) term += W[i-3];
        A[i] = term;

        /* Compute V(i) = Σ_{k=0..i} A(k)*A(i-k) */
        seq_t vi = 0;
        for (idx_t k = 0; k <= i; ++k)
            vi += A[k] * A[i-k];
        V[i] = vi;

        /* Compute W(i) = Σ_{k=0..i} A(k)*V(i-k) */
        seq_t wi = 0;
        for (idx_t k = 0; k <= i; ++k)
            wi += A[k] * V[i-k];
        W[i] = wi;
    }

    last = n;
    return A[n];
}

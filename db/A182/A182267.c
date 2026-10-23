/*
 * A182267  G.f. satisfies: A(x) = (1+x*A(x))*(1+x^2*A(x)^2)*(1+x^3*A(x)).
 * https://oeis.org/A182267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 Recurrence (using convolution s(k) = Σ_{i=0}^{k}
 *            a(i)*a(k-i)): p(k) = a(k-1) + a(k-3) + s(k-4) (terms outside
 *            range are 0) a(n) = p(n) + Σ_{t=0}^{n-2} p(t) * s(n-2-t) for n ≥
 *            1 This generates OEIS sequence A182267
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n) {
    /* static memoisation buffers */
    static seq_t *A = NULL;
    static seq_t *S = NULL;
    static seq_t *P = NULL;
    static idx_t cap = 0;
    static idx_t last = (idx_t)-1;   /* index of last computed term */

    /* grow capacity if necessary */
    if (cap <= n) {
        idx_t new_cap = cap ? (cap << 1) : 4;
        while (new_cap <= n) new_cap <<= 1;
#if defined(__cplusplus)
        seq_t *newA = new seq_t[new_cap];
        seq_t *newS = new seq_t[new_cap];
        seq_t *newP = new seq_t[new_cap];
        for (idx_t i = 0; i < cap; ++i) {
            newA[i] = A[i];
            newS[i] = S[i];
            newP[i] = P[i];
        }
        delete[] A;
        delete[] S;
        delete[] P;
        A = newA;
        S = newS;
        P = newP;
#else
        A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        S = (seq_t*)realloc(S, new_cap * sizeof(seq_t));
        P = (seq_t*)realloc(P, new_cap * sizeof(seq_t));
#endif
        cap = new_cap;
    }

    /* initialise base case only once */
    if (last == (idx_t)-1) {
        A[0] = (seq_t)1;          /* a(0) */
        S[0] = (seq_t)1;          /* s(0) = a0*a0 */
        P[0] = (seq_t)1;          /* p(0) = 1 */
        last = 0;
    }

    /* compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* sum_{t=0}^{i-2} P[t] * S[i-2-t] */
        seq_t sum = (seq_t)0;
        if (i >= 2) {
            for (idx_t t = 0; t <= i - 2; ++t) {
                sum += P[t] * S[i - 2 - t];
            }
        }

        /* terms a(i-1), a(i-3), s(i-4) */
        seq_t term1 = (i >= 1) ? A[i - 1] : (seq_t)0;
        seq_t term2 = (i >= 3) ? A[i - 3] : (seq_t)0;
        seq_t term3 = (i >= 4) ? S[i - 4] : (seq_t)0;

        /* p_i and a_i */
        seq_t pi = term1 + term2 + term3;
        seq_t ai = pi + sum;

        A[i] = ai;
        P[i] = pi;

        /* compute s[i] = Σ_{k=0}^i a[k] * a[i-k] */
        seq_t si = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            si += A[k] * A[i - k];
        }
        S[i] = si;
    }

    last = n;
    return A[n];
}

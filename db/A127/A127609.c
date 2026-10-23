/*
 * A127609  Sequence arising from the factorization of F(n)= A091914(n-1) and
 *          L(n)= A127262. F(0)=0, F(1)=1, F(n)=2*F(n-1)+12*F(n-2), L(0)=2,
 *          L(1)=2, L(n)=2*L(n-1)+12*L(n-2).
 * https://oeis.org/A127609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For even n, F(n)=∏_{d|n}a(d). For odd n, L(n)=∏_{d|n}a(d). Hence
 *            a(n)=F(n)/∏_{d|n, d<n}a(d) if n is even, a(n)=L(n)/∏_{d|n,
 *            d<n}a(d) if n is odd
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* off‑sets are irrelevant */

    /* Allocate temporary arrays */
    seq_t *F = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *L = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *A = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!F || !L || !A) {                 /* allocation failed */
        if (F) free(F); if (L) free(L); if (A) free(A);
        return (seq_t)0;
    }

    /* Base values for F and L */
    F[0] = (seq_t)0;  F[1] = (seq_t)1;
    L[0] = (seq_t)2;  L[1] = (seq_t)2;

    /* Compute F and L up to n */
    for (idx_t i = 2; i <= n; ++i) {
        F[i] = ((seq_t)2) * F[i - 1] + ((seq_t)12) * F[i - 2];
        L[i] = ((seq_t)2) * L[i - 1] + ((seq_t)12) * L[i - 2];
    }

    /* Initialise known a(1) and a(2) */
    A[0] = (seq_t)0;      /* unused */
    A[1] = (seq_t)2;
    if (n >= 2) A[2] = (seq_t)1;

    /* Compute the remaining a(i) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t prod = (seq_t)1;
        /* iterate over all divisors d of i */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t d1 = d;
                idx_t d2 = i / d;
                if (d1 < i) prod *= A[d1];
                if (d2 < i && d2 != d1) prod *= A[d2];
            }
        }
        if ((i & 1) == 0) {            /* even i */
            A[i] = F[i] / prod;
        } else {                       /* odd i */
            A[i] = L[i] / prod;
        }
    }

    seq_t result = A[n];

    /* Free temporary arrays */
    free(F);
    free(L);
    free(A);

    return result;
}

/*
 * A032128  Number of dyslexic planted planar trees with n nodes.
 * https://oeis.org/A032128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) is defined by the series identity * BIK(A(x)) = 1/2 * (
 *            A/(1-A) + (A + A(x^2)) / (1-A(x^2)) ) * with a(1)=1, and where
 *            BIK(A)=shift(A). * The coefficients satisfy the recurrence: *
 *            a(1)=1, * a(m+1) = (g[m] + T[m]) / 2 (m≥1) * where * g[0] = 1 ,
 *            g[n] = Σ_{k=1..n} a(k) * g[n-k], * h[0] = 1 , h...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence is 1‑based; a(0) is unused */
        return 0;

    /* allocate tables of size n+2 (indices 0..n+1 are used)      */
    size_t sz = (size_t)(n + 2);
    seq_t *A = alloc_seq_array(sz);
    seq_t *G = alloc_seq_array(sz);
    seq_t *H = alloc_seq_array(sz);

    /* initialise base cases */
    A[1]  = 1;        /* a(1) = 1 */
    G[0]  = 1;        /* g[0] = 1 */
    H[0]  = 1;        /* h[0] = 1 */

    if (n > 1) {       /* compute a(2) and initialise g[1],h[1] */
        G[1] = A[1] * G[0];   /* g[1] = a1 * g0 */
        H[1] = 0;              /* h[1] = 0  (no even‑index terms) */
    }

    /* iterative computation up to a[n] */
    for (idx_t m = 1; m < n; ++m) {
        /* S_m = g[m] */
        seq_t S = G[m];

        /* T_m = Σ_{k=1..m}   a(k) * h[m-k]  +  Σ_{k=1..⌊m/2⌋} a(k) * h[m-2k] */
        seq_t T = 0;
        for (idx_t k = 1; k <= m; ++k)
            T += A[k] * H[m - k];

        for (idx_t k = 1; k <= m / 2; ++k)
            T += A[k] * H[m - 2 * k];

        /* a(m+1) = (S + T)/2 */
        A[m + 1] = (S + T) / 2;

        /* update g[m+1] = Σ_{k=1..m+1} a(k) * g[m+1-k] */
        seq_t gm1 = 0;
        for (idx_t k = 1; k <= m + 1; ++k)
            gm1 += A[k] * G[m + 1 - k];
        G[m + 1] = gm1;

        /* update h[m+1] = Σ_{k=1..⌊(m+1)/2⌋} a(k) * h[m+1-2k] */
        seq_t hm1 = 0;
        for (idx_t k = 1; k <= (m + 1) / 2; ++k)
            hm1 += A[k] * H[m + 1 - 2 * k];
        H[m + 1] = hm1;
    }

    seq_t result = A[n];

    /* free allocated arrays */
    seq_free_array(A);
    seq_free_array(G);
    seq_free_array(H);

    return result;
}

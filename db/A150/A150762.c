/*
 * A150762  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 0,
 *          1), (0, 0, -1), (0, 0, 1), (1, 1, 1)}.
 * https://oeis.org/A150762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Σ_{k=0}^{⌊n/2⌋} (C(n,k)−C(n,k−1)) · T(n−k) where T(m) =
 *            Σ_{m'=0}^{m} C(m,m′) · G(m′) G(m′) = Σ_{r=0}^{⌊m′/2⌋} 2^r ·
 *            (C(m′,r)−C(m′,r−1)) All binomial coefficients are computed
 *            multiplicatively to avoid factorial overflow. This method runs
 *            in O(n²) time and uses only integer arithmetic. It w...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*----------------------------------------------------------------------*/
/*  Compute A150762 for a given n.                                     */
/*----------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Pre‑allocate working arrays.  */
    size_t N = (size_t)n;
    seq_t *pow2 = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *G    = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *T    = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!pow2 || !G || !T) { free(pow2); free(G); free(T); return 0; }

    /* 2^i  */
    pow2[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    /* Compute G[m] for all m.  */
    for (size_t m = 0; m <= N; ++m) {
        seq_t cur  = (seq_t)1;          /* C(m,0)   */
        seq_t prev = (seq_t)0;          /* C(m,-1)  */
        seq_t sum  = (seq_t)0;
        size_t rmax = m / 2;
        for (size_t r = 0; r <= rmax; ++r) {
            seq_t ballot = cur - prev;                    /* C(m,r)‑C(m,r‑1) */
            seq_t term   = pow2[r] * ballot;             /* 2^r · ballot  */
            sum += term;
            prev = cur;
            if (r < rmax)
                cur = cur * (seq_t)(m - r) / (seq_t)(r + 1); /* next binomial */
        }
        G[m] = sum;
    }

    /* Compute T[m] for all m.  */
    for (size_t m = 0; m <= N; ++m) {
        seq_t cur  = (seq_t)1;      /* C(m,0)   */
        seq_t sum  = (seq_t)0;
        for (size_t mp = 0; mp <= m; ++mp) {
            seq_t binom = cur;      /* C(m,mp)  */
            sum += binom * G[mp];
            if (mp < m)
                cur = cur * (seq_t)(m - mp) / (seq_t)(mp + 1); /* next binomial */
        }
        T[m] = sum;
    }

    /* Compute a(n).  */
    seq_t result      = (seq_t)0;
    seq_t cur_binom   = (seq_t)1;  /* C(n,0)   */
    seq_t prev_binom  = (seq_t)0;  /* C(n,-1)  */
    size_t half = n / 2;
    for (size_t k = 0; k <= half; ++k) {
        seq_t ballot_z = cur_binom - prev_binom;   /* C(n,k)‑C(n,k‑1)   */
        size_t m = n - k;
        result += ballot_z * T[m];
        if (k < half) {
            seq_t next_binom = cur_binom * (seq_t)(n - k) /
                               (seq_t)(k + 1);       /* C(n,k+1)  */
            prev_binom = cur_binom;
            cur_binom  = next_binom;
        }
    }

    free(pow2);
    free(G);
    free(T);
    return result;
}

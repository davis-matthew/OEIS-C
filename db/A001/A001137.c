/*
 * A001137  Number of black-rooted red-black trees with n internal nodes.
 * https://oeis.org/A001137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count by leaves. With f(t) = (t+t^2)^2, the g.f. of black-rooted
 *            red-black trees by number of leaves is sum_{h>=1} f^(h)(x)
 *            (h-fold iterate). Let S(N) = [x^N] of that sum. Then S(N) =
 *            Sum_{m=ceil(N/4)..floor(N/2)} binomial(2m, N-2m) * ([m=1] +
 *            S(m)), and a(n) = S(n+1). This is equivalent to A(x)...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 63, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a001137_S = NULL;
static idx_t a001137_cap = 0;

static void a001137_build(idx_t Nmax) {
    seq_t *S = (seq_t *)calloc(Nmax + 1, sizeof(seq_t));
    seq_t *P = (seq_t *)calloc(Nmax + 1, sizeof(seq_t));
    idx_t r = 0, m, k, t;
    P[0] = 1;
    for (m = 1; 2 * m <= Nmax; m++) {
        idx_t K = Nmax - 2 * m;
        seq_t D;
        /* advance Pascal row from r = 2m-2 to 2m, keeping entries k <= K */
        for (t = 0; t < 2; t++) {
            idx_t top = r + 1 < K ? r + 1 : K;
            for (k = top; k >= 1; k--) P[k] += P[k - 1];
            r++;
        }
        D = S[m];
        if (m == 1) D += 1;
        if (D == 0) continue;
        {
            idx_t lim = 2 * m < K ? 2 * m : K;
            for (k = 0; k <= lim; k++) S[2 * m + k] += D * P[k];
        }
    }
    free(P);
    if (a001137_S) free(a001137_S);
    a001137_S = S;
    a001137_cap = Nmax;
}

seq_t a(idx_t n) {
    idx_t N = n + 1;
    if (n == 0) return 0;
    if (a001137_S == NULL || N > a001137_cap) {
        idx_t want = 2 * a001137_cap;
        if (want < N) want = N;
        if (want < 16) want = 16;
        a001137_build(want);
    }
    return a001137_S[N];
}

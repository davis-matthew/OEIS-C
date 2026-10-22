/*
 * A001036  Partial sums of A001037, omitting A001037(1).
 * https://oeis.org/A001036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=2..n} A001037(k), where A001037(k) =
 *            (1/k)*Sum_{d|k} mu(d)*2^(k/d)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *P = NULL;   /* P[e] = 2^e */
static seq_t *S = NULL;   /* S[k] = a(k) */
static int *MU = NULL;
static idx_t have = 0;    /* S[1..have] valid */
static idx_t cap = 0;
static int fixedw = -1;

static void build_mu(idx_t m) {
    char *comp = (char *)calloc((size_t)m + 1, 1);
    idx_t i, j;
    for (i = 0; i <= m; i++) MU[i] = 1;
    for (i = 2; i <= m; i++) {
        if (comp[i]) continue;
        for (j = i; j <= m; j += i) {
            if (j > i) comp[j] = 1;
            MU[j] = -MU[j];
        }
        if (i <= m / i)
            for (j = i * i; j <= m; j += i * i) MU[j] = 0;
    }
    free(comp);
}

static seq_t lyndon(idx_t k) {
    idx_t v = 0, odd = k, d;
    seq_t pos = 0, neg = 0, M;
    while ((odd & 1) == 0) { odd >>= 1; v++; }
    for (d = 1; d <= k / d; d++) {
        if (k % d != 0) continue;
        {
            idx_t d2 = k / d;
            if (MU[d] > 0) pos += P[d2 - v];
            else if (MU[d] < 0) neg += P[d2 - v];
            if (d2 != d) {
                if (MU[d2] > 0) pos += P[d - v];
                else if (MU[d2] < 0) neg += P[d - v];
            }
        }
    }
    M = pos - neg;
    if (fixedw) {
        seq_t q = (seq_t)odd, x = (seq_t)odd;
        int it;
        for (it = 0; it < 8; it++) x = x * ((seq_t)2 - q * x);
        return M * x;
    }
    return M / (seq_t)odd;
}

static void grow(idx_t n) {
    idx_t newcap = cap ? cap : 16, i, k;
    seq_t *NP, *NS;
    while (newcap < n) newcap *= 2;
    NP = (seq_t *)malloc(sizeof(seq_t) * ((size_t)newcap + 1));
    NS = (seq_t *)malloc(sizeof(seq_t) * ((size_t)newcap + 1));
    for (i = 0; i <= newcap; i++) {
        NP[i] = 0;
        NS[i] = 0;
    }
    NP[0] = 1;
    for (i = 1; i <= newcap; i++) NP[i] = NP[i - 1] + NP[i - 1];
    for (i = 1; i <= have; i++) NS[i] = S[i];
    if (P) {
        for (i = 0; i <= cap; i++) { P[i] = 0; S[i] = 0; }
        free(P);
        free(S);
    }
    P = NP;
    S = NS;
    free(MU);
    MU = (int *)malloc(sizeof(int) * ((size_t)newcap + 1));
    cap = newcap;
    build_mu(cap);
    if (have == 0) {
        S[1] = 1;
        have = 1;
    }
    for (k = have + 1; k <= cap; k++) S[k] = S[k - 1] + lyndon(k);
    have = cap;
}

seq_t a(idx_t n) {
    if (fixedw < 0) {
        seq_t t = 1;
        int i;
        for (i = 0; i < 200; i++) t = t + t;
        fixedw = (t == 0) ? 1 : 0;
    }
    if (n > have) grow(n);
    return S[n];
}

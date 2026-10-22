/*
 * A000014  Number of series-reduced trees with n nodes.
 * https://oeis.org/A000014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   Otter's dissimilarity theorem. P(x) = g.f. of A001678 (planted
 *            series-reduced trees): P(x) = x*(E(x) - P(x)), where E = Euler
 *            transform of P, i.e. E(x) = exp(Sum_{k>=1} P(x^k)/k). So P(0)=0,
 *            P(n) = E(n-1) - P(n-1), E(0)=1, n*E(n) = Sum_{k=1..n}
 *            c(k)*E(n-k), c(k) = Sum_{d|k} d*P(d). a(0)=0; for n...
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 70, u128 n <= 129,
 *            big any n
 * Helpers:   c2, finish, inv_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/c2.h"
#include "../utils/finish.h"
#include "../utils/inv_odd.h"

#define BMAX 20001
#define FMAX 300

static seq_t BP[BMAX], BE[BMAX], BC[BMAX];
static idx_t bdone = 0;
static bool binit = false;

static seq_t FP[FMAX], FE[FMAX], FB[FMAX];

static seq_t a_fixed(idx_t n)
{
    seq_t t = 0;
    int w = 0;
    idx_t N = n, d, i, j, J;
    t -= 1;
    while (t != 0) { t >>= 1; w++; }
    if (n > (idx_t)(2 * w + 40)) return 0; /* cannot fit in seq_t */
    for (i = 0; i <= N; i++) FE[i] = 0;
    FE[0] = 1;
    FP[0] = 0;
    for (d = 1; d <= N; d++) {
        seq_t O = 1;
        int e = 0;
        FP[d] = FE[d - 1] - FP[d - 1];
        if (FP[d] == 0) continue;
        J = N / d;
        /* FB[j] = binomial(P(d)+j-1, j) mod 2^w, tracked as odd part * 2^e */
        for (j = 1; j <= J; j++) {
            idx_t q = j;
            t = FP[d] + (seq_t)(j - 1);
            if (t == 0) {
                e += 4 * w;
            } else {
                while ((t & 1) == 0) { t >>= 1; e++; }
                O *= t;
            }
            while ((q & 1) == 0) { q >>= 1; e--; }
            O *= inv_odd((seq_t)q);
            if (e < 0) e = 0;
            if (e > 8 * w) e = 8 * w;
            FB[j] = (e >= w) ? (seq_t)0 : (seq_t)(O << e);
        }
        for (i = N; i >= d; i--) {
            seq_t s = 0;
            for (j = 1; j * d <= i; j++) s += FB[j] * FE[i - j * d];
            FE[i] += s;
        }
    }
    return finish(FP, FE, n);
}

static seq_t a_big(idx_t n)
{
    idx_t m, k;
    if (n >= BMAX) return 0;
    if (!binit) {
        BE[0] = 1;
        BP[0] = 0;
        for (k = 0; k < BMAX; k++) BC[k] = 0;
        bdone = 0;
        binit = true;
    }
    for (m = bdone + 1; m <= n; m++) {
        seq_t mp, s = 0;
        BP[m] = BE[m - 1] - BP[m - 1];
        mp = BP[m] * (seq_t)m;
        for (k = m; k < BMAX; k += m) BC[k] += mp;
        for (k = 1; k <= m; k++) s += BC[k] * BE[m - k];
        BE[m] = s / (seq_t)m;
        bdone = m;
    }
    return finish(BP, BE, n);
}

seq_t a(idx_t n)
{
    seq_t z = 0;
    z -= 1;
    if (z > 0) return a_fixed(n);
    return a_big(n);
}

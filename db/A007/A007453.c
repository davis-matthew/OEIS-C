/*
 * A007453  Number of unlabeled connected series-parallel posets with n nodes.
 * https://oeis.org/A007453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Inverse Euler transform of A003430, i.e. A003430 = Euler(a). Let
 *            P = A003430 (all series-parallel posets), D(n) = P(n) - a(n) =
 *            [x^n] prod_{k<n} (1-x^k)^(-a(k)) (disconnected posets, D(1) =
 *            0), S(1) = 1, S(n) = D(n) for n > 1 (series-indecomposable
 *            posets). Then P = S/(1-S), so P(n) = S(n) + Sum_...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 63,
 *            big any n
 * Helpers:   alloc_zero, release, sp_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/sp_gcd.h"

static seq_t *spC = 0, *spP = 0, *spS = 0, *spB = 0, *spM = 0;
static idx_t spL = 0;

static void sp_build(idx_t N)
{
    idx_t m, n, k, i, j;
    if (spL > 0) {
        release(spC, spL + 1);
        release(spP, spL + 1);
        release(spS, spL + 1);
        release(spB, spL + 1);
        release(spM, spL + 1);
    }
    spC = alloc_zero(N + 1);
    spP = alloc_zero(N + 1);
    spS = alloc_zero(N + 1);
    spB = alloc_zero(N + 1);
    spM = alloc_zero(N + 1);
    spL = N;
    spB[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t E = spB[m];
        idx_t J;
        if (m == 1) spS[m] = 1;
        else spS[m] = E;
        spP[m] = spS[m];
        for (k = 1; k < m; k++) spP[m] += spS[k] * spP[m - k];
        if (m == 1) spC[m] = spP[m];
        else spC[m] = spP[m] - E;
        /* multiply B by (1 - x^m)^(-C[m]) */
        J = N / m;
        spM[0] = 1;
        for (i = 1; i <= J; i++) {
            seq_t r = spM[i - 1];
            seq_t t = spC[m] + (seq_t)(i - 1);
            seq_t si = (seq_t)i;
            seq_t g = sp_gcd(r, si);
            spM[i] = (r / g) * (t / (si / g));
        }
        for (n = N; n >= m; n--) {
            seq_t acc = spB[n];
            for (j = 1; j * m <= n; j++) acc += spM[j] * spB[n - j * m];
            spB[n] = acc;
        }
    }
}

seq_t a(idx_t n)
{
    if (n > spL) {
        idx_t N = 2 * spL;
        if (N < n) N = n;
        if (N < 32) N = 32;
        sp_build(N);
    }
    return spC[n];
}

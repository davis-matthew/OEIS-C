/*
 * A007710  From the game of Mousetrap.
 * https://oeis.org/A007710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A028305((1/2)*(n+1)*(n+2)-n+1), the number of
 *            permutations of n cards with exactly one hit in Mousetrap.
 *            Computed by summing over the first-hit position k, then counting
 *            the rest by inclusion-exclusion with rook polynomials: sum_j
 *            (-1)^j r_j (n-1-j)!. The forbidden board splits into paths...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   a7710_gcd, alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/a7710_gcd.h"

seq_t a(idx_t n) {
    idx_t N = n - 1;
    idx_t m, i, t, j, k, c, step;
    /* k = n: derangements D(n-1) */
    seq_t total = 1;
    for (m = 1; m <= N; m++) {
        total = total * (seq_t)m;
        if (m & 1) total -= 1; else total += 1;
    }
    if (n < 2) return total;

    idx_t asz = 2 * n + 4, psz = n + 4, hsz = 2 * n + 4;
    seq_t *f = alloc_zero(N + 1);
    seq_t *A = alloc_zero(asz);
    seq_t *Pa = alloc_zero(psz);
    seq_t *Pb = alloc_zero(psz);
    seq_t *Pc = alloc_zero(psz);
    idx_t *hist = (idx_t *)calloc((size_t)hsz, sizeof(idx_t));

    f[0] = 1;
    for (i = 1; i <= N; i++) f[i] = f[i - 1] * (seq_t)i;

    for (k = 1; k < n; k++) {
        idx_t g = a7710_gcd(n, k), L = n / g;
        idx_t twok = (2 * k) % n, kp1 = (k + 1) % n;
        idx_t maxm = 0;
        for (i = 0; i < hsz; i++) hist[i] = 0;

        for (c = 0; c < g; c++) {
            idx_t r = n - g + c, cur = 0, bonus = 0;
            for (step = 0; step < L; step++) {
                /* identity edge P_r - V_r */
                if (r >= 1 && r < k) {
                    cur++;
                } else if (cur) {
                    hist[cur + bonus]++;
                    if (cur + bonus > maxm) maxm = cur + bonus;
                    cur = 0; bonus = 0;
                }
                /* shift edge P_r - V_{r-k} */
                if (r != k && r != twok) {
                    if (cur == 0 && r == kp1) bonus = 1;
                    cur++;
                } else if (cur) {
                    hist[cur + bonus]++;
                    if (cur + bonus > maxm) maxm = cur + bonus;
                    cur = 0; bonus = 0;
                }
                r = (r + n - k) % n;
            }
            if (cur) {
                hist[cur + bonus]++;
                if (cur + bonus > maxm) maxm = cur + bonus;
            }
        }
        /* extra edge P_{k+1} - V_n isolated when shift edge at k+1 is absent */
        if (kp1 == twok || kp1 == k) {
            hist[1]++;
            if (maxm < 1) maxm = 1;
        }

        for (i = 0; i < asz; i++) A[i] = 0;
        A[0] = 1;
        idx_t degA = 0;

        /* Pa = P_{m-2}, Pb = P_{m-1} */
        for (i = 0; i < psz; i++) { Pa[i] = 0; Pb[i] = 0; }
        Pa[0] = 1;            /* P_0 */
        Pb[0] = 1; Pb[1] = 1; /* P_1 */
        for (m = 1; m <= maxm; m++) {
            seq_t *P;
            if (m == 1) {
                P = Pb;
            } else {
                idx_t d1 = m / 2, d2 = (m - 1) / 2, dn = (m + 1) / 2;
                for (j = 0; j <= dn; j++) {
                    seq_t v = 0;
                    if (j <= d1) v += Pb[j];
                    if (j >= 1 && j - 1 <= d2) v += Pa[j - 1];
                    Pc[j] = v;
                }
                for (j = dn + 1; j < psz; j++) Pc[j] = 0;
                { seq_t *tmp = Pa; Pa = Pb; Pb = Pc; Pc = tmp; }
                P = Pb;
            }
            if (hist[m]) {
                idx_t dm = (m + 1) / 2, rep;
                for (rep = 0; rep < hist[m]; rep++) {
                    i = degA + 1;
                    while (i-- > 0) {
                        for (t = dm; t >= 1; t--) A[i + t] += A[i] * P[t];
                    }
                    degA += dm;
                }
            }
        }

        for (j = 0; j <= degA && j <= N; j++) {
            seq_t term = A[j] * f[N - j];
            if (j & 1) total -= term; else total += term;
        }
    }

    release(f, N + 1);
    release(A, asz);
    release(Pa, psz);
    release(Pb, psz);
    release(Pc, psz);
    free(hist);
    return total;
}

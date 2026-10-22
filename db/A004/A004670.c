/*
 * A004670  Theta series of extremal even unimodular lattice in dimension 32.
 * https://oeis.org/A004670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A282012(n) - 960*A027364(n-1) for n > 0, a(0) = 1, where
 *            A282012 = E4^4 = E8^2 with E8 = 1 + 480*Sum sigma_7(n) q^n, and
 *            A027364 is the q-expansion of E4*Delta, E4 = 1 + 240*Sum
 *            sigma_3(n) q^n, Delta = q*Prod (1-q^k)^24 = q*J^8, J = Prod
 *            (1-q^k)^3 = Sum (-1)^k (2k+1) q^(k(k+1)/2). Signed q...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 17, u128 n <= 335,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_zero.h"

static idx_t cap = 0;            /* tables valid for indices 0..cap-1 */
static seq_t *s3 = 0, *s7 = 0;   /* sigma_3, sigma_7 */
static seq_t *jp = 0, *jm = 0;   /* J^8 positive / negative parts */
static seq_t *tp = 0, *tm = 0;   /* temporaries */

static void build(idx_t L)
{
    idx_t i, d, m, k, rep;
    s3 = grow_zero(s3, cap, L);
    s7 = grow_zero(s7, cap, L);
    jp = grow_zero(jp, cap, L);
    jm = grow_zero(jm, cap, L);
    tp = grow_zero(tp, cap, L);
    tm = grow_zero(tm, cap, L);
    cap = L;
    for (i = 0; i < L; i++) { s3[i] = 0; s7[i] = 0; jp[i] = 0; jm[i] = 0; }
    for (d = 1; d < L; d++) {
        seq_t dd = (seq_t)d;
        seq_t d3 = dd * dd * dd;
        seq_t d7 = d3 * d3 * dd;
        for (m = d; m < L; m += d) { s3[m] += d3; s7[m] += d7; }
    }
    jp[0] = 1;
    for (rep = 0; rep < 8; rep++) {
        seq_t *sw;
        for (i = 0; i < L; i++) { tp[i] = 0; tm[i] = 0; }
        for (k = 0; k * (k + 1) / 2 < L; k++) {
            idx_t t = k * (k + 1) / 2;
            seq_t c = (seq_t)(2 * k + 1);
            if ((k & 1) == 0) {
                for (i = 0; i + t < L; i++) {
                    tp[i + t] += c * jp[i];
                    tm[i + t] += c * jm[i];
                }
            } else {
                for (i = 0; i + t < L; i++) {
                    tp[i + t] += c * jm[i];
                    tm[i + t] += c * jp[i];
                }
            }
        }
        sw = jp; jp = tp; tp = sw;
        sw = jm; jm = tm; tm = sw;
    }
}

seq_t a(idx_t n)
{
    idx_t j;
    seq_t S, Bp, Bm;
    if (n == 0) return 1;
    if (n + 1 > cap) {
        idx_t L = 2 * cap;
        if (L < n + 1) L = n + 1;
        if (L < 64) L = 64;
        build(L);
    }
    /* E8^2 coefficient */
    S = (seq_t)960 * s7[n];
    for (j = 1; j < n; j++) S += (seq_t)230400 * s7[j] * s7[n - j];
    /* (E4*Delta) coefficient at q^n */
    Bp = 0; Bm = 0;
    for (j = 1; j <= n; j++) {
        seq_t e = (j == n) ? (seq_t)1 : (seq_t)240 * s3[n - j];
        Bp += jp[j - 1] * e;
        Bm += jm[j - 1] * e;
    }
    return S + (seq_t)960 * Bm - (seq_t)960 * Bp;
}

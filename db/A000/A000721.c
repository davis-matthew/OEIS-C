/*
 * A000721  Number of NP-equivalence classes of balanced Boolean functions of
 *          n variables.
 * https://oeis.org/A000721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Burnside's lemma over the hyperoctahedral group B_n (order
 *            2^n*n!): a(n) = (1/(2^n*n!)) * Sum_{signed cycle types c}
 *            |class(c)| * [x^(2^(n-1))] Prod_{cycles C of c on {0,1}^n} (1 +
 *            x^|C|)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   gcdu, mu_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/gcdu.h"
#include "../utils/mu_small.h"

#define MAXS 8192
static int ci[64], cs[64], nc;
static idx_t NN;
static uint64_t HALF;
static int mode128;
static uint64_t SL[MAXS], SC[MAXS], TL[MAXS], TC[MAXS];
static int ns;
static unsigned __int128 Q[65], T128;
static seq_t P[MAXS + 1];
static seq_t TS;

/* combine current structure S with the action of one signed i-cycle on {0,1}^i */
static void add_dist(uint64_t i, int s) {
    uint64_t per = s ? 2 * i : i;
    uint64_t dl[256]; uint64_t dc[256]; int nd = 0;
    for (uint64_t d = 1; d <= per; d++) {
        if (per % d) continue;
        int64_t sum = 0;
        for (uint64_t e = 1; e <= d; e++) {
            if (d % e) continue;
            int m = mu_small(d / e);
            if (!m) continue;
            uint64_t c = gcdu(e, i);
            int64_t F = (s && ((e / c) & 1)) ? 0 : ((int64_t)1 << c);
            sum += m * F;
        }
        if (sum > 0) { dl[nd] = d; dc[nd] = (uint64_t)sum / d; nd++; }
    }
    int nt = 0;
    for (int a = 0; a < ns; a++) {
        for (int b = 0; b < nd; b++) {
            uint64_t g = gcdu(SL[a], dl[b]);
            uint64_t L = SL[a] / g * dl[b];
            if (L > HALF) continue; /* lengths only grow; never reaches degree HALF */
            uint64_t c = g * SC[a] * dc[b];
            int f = -1;
            for (int t = 0; t < nt; t++) if (TL[t] == L) { f = t; break; }
            if (f >= 0) TC[f] += c;
            else { TL[nt] = L; TC[nt] = c; nt++; }
        }
    }
    for (int t = 0; t < nt; t++) { SL[t] = TL[t]; SC[t] = TC[t]; }
    ns = nt;
}

static void leaf(void) {
    ns = 1; SL[0] = 1; SC[0] = 1;
    for (int k = 0; k < nc && ns > 0; k++) add_dist((uint64_t)ci[k], cs[k]);
    if (ns == 0) return;
    if (mode128) {
        unsigned __int128 cls = 1;
        for (idx_t k = 1; k <= NN; k++) cls *= (unsigned __int128)(2 * k);
        int run = 0;
        for (int k = 0; k < nc; k++) {
            run = (k > 0 && ci[k] == ci[k - 1] && cs[k] == cs[k - 1]) ? run + 1 : 1;
            cls /= (unsigned __int128)(2 * ci[k]);
            cls /= (unsigned __int128)run;
        }
        for (uint64_t j = 0; j <= HALF; j++) Q[j] = 0;
        Q[0] = 1;
        for (int a = 0; a < ns; a++) {
            uint64_t L = SL[a];
            for (uint64_t r = 0; r < SC[a]; r++)
                for (uint64_t k = HALF; k >= L; k--) Q[k] += Q[k - L];
        }
        T128 += Q[HALF] * cls;
    } else {
        seq_t cls = 1;
        for (idx_t k = 1; k <= NN; k++) cls *= (idx_t)(2 * k);
        int run = 0;
        for (int k = 0; k < nc; k++) {
            run = (k > 0 && ci[k] == ci[k - 1] && cs[k] == cs[k - 1]) ? run + 1 : 1;
            cls /= (idx_t)(2 * ci[k]);
            cls /= (idx_t)run;
        }
        for (uint64_t j = 0; j <= HALF; j++) P[j] = 0;
        P[0] = 1;
        for (int a = 0; a < ns; a++) {
            uint64_t L = SL[a];
            for (uint64_t r = 0; r < SC[a]; r++)
                for (uint64_t k = HALF; k >= L; k--) P[k] += P[k - L];
        }
        TS += P[HALF] * cls;
    }
}

/* enumerate multisets of signed cycles (key = 2*i + sign) in nonincreasing key order */
static void gen(idx_t rem, int maxkey) {
    if (rem == 0) { leaf(); return; }
    for (int key = maxkey; key >= 2; key--) {
        idx_t i = (idx_t)(key / 2);
        if (i > rem) continue;
        ci[nc] = (int)i; cs[nc] = key % 2; nc++;
        gen(rem - i, key);
        nc--;
    }
}

seq_t a(idx_t n) {
    if (n < 1 || n > 14) return 0;
    NN = n;
    HALF = (uint64_t)1 << (n - 1);
    mode128 = (n <= 7);
    nc = 0;
    T128 = 0;
    TS = 0;
    gen(n, (int)(2 * n + 1));
    if (mode128) {
        unsigned __int128 G = 1;
        for (idx_t k = 1; k <= n; k++) G *= (unsigned __int128)(2 * k);
        unsigned __int128 v = T128 / G;
        uint64_t hi = (uint64_t)(v >> 64), lo = (uint64_t)v;
        seq_t r = 0;
        if (hi) {
            r = (seq_t)hi;
            r = r * 4294967296ULL;
            r = r * 4294967296ULL;
        }
        r += (seq_t)lo;
        return r;
    } else {
        seq_t G = 1;
        for (idx_t k = 1; k <= n; k++) G *= (idx_t)(2 * k);
        return TS / G;
    }
}

/*
 * A000571  Number of different score sequences that are possible in an n-team
 *          round-robin tournament.
 * https://oeis.org/A000571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = 1, a(n) = (1/n) * Sum_{k=1..n} A145855(k)*a(n-k), where
 *            A145855(k) = (1/(2k)) * Sum_{d|k} (-1)^(k+d) * phi(k/d) *
 *            C(2d,d). For fixed-width seq_t (where only small n can fit) a
 *            division-free count is used instead: a DP over nondecreasing
 *            score sequences s_1<=...<=s_n with s_1+...+s_j >= C(j...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 39, u128 n <= 72,
 *            big any n
 * Helpers:   alloc_zero, fixed571, phi571
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/fixed571.h"
#include "../utils/phi571.h"

static seq_t dp571(idx_t n) {
    long long N = (long long)n, T = N * (N - 1) / 2, W = T + 1;
    long long v, S, w, j, i;
    seq_t *cur = alloc_zero((idx_t)(N * W));
    seq_t *nxt = alloc_zero((idx_t)(N * W));
    seq_t *tmp, run, res;
    for (v = 0; v < N; v++) cur[v * W + v] = 1;
    for (j = 2; j <= N; j++) {
        long long need = j * (j - 1) / 2;
        for (i = 0; i < N * W; i++) nxt[i] = 0;
        for (S = 0; S <= T; S++) {
            run = 0;
            for (w = 0; w < N; w++) {
                long long t = S + w;
                run += cur[w * W + S];
                if (t >= need && t <= T) nxt[w * W + t] = run;
            }
        }
        tmp = cur; cur = nxt; nxt = tmp;
    }
    res = 0;
    for (v = 0; v < N; v++) res += cur[v * W + T];
    free(cur);
    free(nxt);
    return res;
}

static seq_t *Am = 0, *Bm = 0, *Cm = 0;
static idx_t cap571 = 0, have571 = 0;

static void addterm571(idx_t k, idx_t d, seq_t *pos, seq_t *neg) {
    seq_t s = Cm[d] * phi571(k / d);
    if ((k + d) % 2 == 0) *pos += s; else *neg += s;
}

static void ensure571(idx_t N) {
    idx_t k, i, d;
    if (N + 1 > cap571) {
        idx_t nc = 2 * cap571;
        seq_t *na, *nb, *ncc;
        if (nc < N + 1) nc = N + 1;
        na = alloc_zero(nc);
        nb = alloc_zero(nc);
        ncc = alloc_zero(nc);
        for (i = 0; i < have571; i++) {
            na[i] = Am[i];
            nb[i] = Bm[i];
            ncc[i] = Cm[i];
        }
        Am = na; Bm = nb; Cm = ncc;
        cap571 = nc;
    }
    for (k = have571; k <= N; k++) {
        if (k == 0) {
            Am[0] = 1; Bm[0] = 0; Cm[0] = 1;
        } else {
            seq_t pos = 0, neg = 0, s = 0;
            Cm[k] = Cm[k - 1] * (4 * k - 2) / k;
            for (d = 1; d * d <= k; d++) {
                if (k % d == 0) {
                    addterm571(k, d, &pos, &neg);
                    if (d * d != k) addterm571(k, k / d, &pos, &neg);
                }
            }
            Bm[k] = (pos - neg) / ((seq_t)k * 2);
            for (i = 1; i <= k; i++) s += Bm[i] * Am[k - i];
            Am[k] = s / k;
        }
        have571 = k + 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (fixed571()) {
        /* a(100) > 2^170, so larger n cannot fit in any fixed-width seq_t */
        if (n > 100) return 0;
        return dp571(n);
    }
    if (n >= have571) ensure571(n);
    return Am[n];
}

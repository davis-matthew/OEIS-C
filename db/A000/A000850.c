/*
 * A000850  Number of n-input 2-output switching networks under action of
 *          AG(n,2) on the inputs and complementing group C(2,2) on the
 *          outputs.
 * https://oeis.org/A000850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(4*|AG(n,2)|)) * Sum_{g in AG(n,2)} 4^c(g) * (1 +
 *            3*[every cycle of g has even length]), where c(g) is the number
 *            of cycles of g acting on GF(2)^n (Burnside's lemma with the
 *            complementing group C(2,2)). The sum is taken over conjugacy
 *            classes of GL(n,2) (rational canonical form) and ove...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a850_deg, a850_gcd, a850_lcm, a850_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a850_deg.h"
#include "../utils/a850_gcd.h"
#include "../utils/a850_lcm.h"
#include "../utils/a850_mod.h"

static int a850_n, a850_big, a850_np, a850_sp;
static uint64_t *a850_pord;
static int *a850_pdeg, *a850_poff, *a850_plen, *a850_pst;
static seq_t a850_GLs, a850_accs;
static unsigned __int128 a850_GLw, a850_accw;

static void a850_process(int lim)
{
    int i, j, k;
    seq_t cs = 1, clss = 0;
    unsigned __int128 cw = 1, clsw = 0;
    for (i = 0; i < lim; i++) {
        int len = a850_plen[i];
        int *pp, d, maxp;
        long long E = 0;
        if (!len) continue;
        pp = a850_pst + a850_poff[i];
        d = a850_pdeg[i];
        maxp = pp[0];
        for (j = 1; j <= maxp; j++) {
            long long c = 0;
            for (k = 0; k < len; k++) if (pp[k] >= j) c++;
            E += c * c;
        }
        for (j = 1; j <= maxp; j++) {
            long long mv = 0;
            for (k = 0; k < len; k++) if (pp[k] == j) mv++;
            E -= mv * (mv + 1) / 2;
            for (k = 1; k <= mv; k++) {
                if (a850_big) cs *= ((seq_t)1 << (d * k)) - 1;
                else cw *= (((unsigned __int128)1) << (d * k)) - 1;
            }
        }
        if (a850_big) cs <<= (int)(d * E);
        else cw <<= (int)(d * E);
    }
    if (a850_big) clss = a850_GLs / cs;
    else clsw = a850_GLw / cw;

    /* multiple of the element order */
    uint64_t L = 1, P2 = 1, M;
    for (i = 1; i < lim; i++) {
        uint64_t tp = 1;
        if (!a850_plen[i]) continue;
        while (tp < (uint64_t)a850_pst[a850_poff[i]]) tp *= 2;
        L = a850_lcm(L, a850_pord[i] * tp);
    }
    int *mu = a850_pst + a850_poff[0];
    int mlen = a850_plen[0];
    int m = 0, maxmu = mlen ? mu[0] : 0;
    for (k = 0; k < mlen; k++) m += mu[k];
    while (P2 < (uint64_t)m + 1) P2 *= 2;
    M = a850_lcm(L, P2);

    int prevD = -1, s0;
    for (s0 = 0;; s0++) {
        long long pw = 1LL << s0;
        int D = m;
        for (k = 0; k < mlen; k++) {
            D += (mu[k] < pw - 1 ? mu[k] : (int)(pw - 1));
            D -= (mu[k] < pw ? mu[k] : (int)pw);
        }
        if (D != prevD) {
            uint64_t sum = 0, kk, c;
            for (kk = 1; kk <= M; kk++) {
                int s = 0, ex = 0;
                uint64_t o = kk;
                long long cap;
                while (!(o & 1)) { o >>= 1; s++; }
                if (s < s0) continue;
                cap = s >= 30 ? (1LL << 30) : (1LL << s);
                for (k = 0; k < mlen; k++) ex += (mu[k] < cap ? mu[k] : (int)cap);
                for (i = 1; i < lim; i++) {
                    int len = a850_plen[i];
                    int *pp;
                    if (!len || o % a850_pord[i] != 0) continue;
                    pp = a850_pst + a850_poff[i];
                    for (k = 0; k < len; k++)
                        ex += a850_pdeg[i] * (pp[k] < cap ? pp[k] : (int)cap);
                }
                sum += (uint64_t)1 << ex;
            }
            c = sum / M;
            /* 4^(c-1) * (1 + 3e), times 2^(n-m) translations on the non-unipotent part */
            int shift = (int)(2 * (c - 1)) + (s0 >= 1 ? 2 : 0) + (a850_n - m);
            if (a850_big) {
                seq_t cnt = (seq_t)1 << D;
                if (prevD >= 0) cnt -= (seq_t)1 << prevD;
                a850_accs += (clss * cnt) << shift;
            } else {
                unsigned __int128 cnt = ((unsigned __int128)1) << D;
                if (prevD >= 0) cnt -= ((unsigned __int128)1) << prevD;
                a850_accw += (clsw * cnt) << shift;
            }
        }
        prevD = D;
        if (pw - 1 >= maxmu) break;
    }
}

static void a850_poly(int i, int rem);

static void a850_part(int i, int s, int maxp, int rem_after)
{
    int v;
    if (s == 0) { a850_poly(i + 1, rem_after); return; }
    for (v = (s < maxp ? s : maxp); v >= 1; v--) {
        a850_pst[a850_sp++] = v;
        a850_plen[i]++;
        a850_part(i, s - v, v, rem_after);
        a850_sp--;
        a850_plen[i]--;
    }
}

static void a850_poly(int i, int rem)
{
    int s, d;
    if (rem == 0) { a850_process(i); return; }
    if (i >= a850_np || a850_pdeg[i] > rem) return;
    d = a850_pdeg[i];
    a850_plen[i] = 0;
    a850_poff[i] = a850_sp;
    a850_poly(i + 1, rem);
    for (s = 1; s * d <= rem; s++) {
        a850_poff[i] = a850_sp;
        a850_plen[i] = 0;
        a850_part(i, s, s, rem - s * d);
    }
    a850_plen[i] = 0;
}

seq_t a(idx_t n)
{
    int i, d, cap;
    uint64_t f, g;
    seq_t x = 1, r;
    if (n == 0) return 0;
    for (i = 0; i < 200; i++) x <<= 1;
    a850_big = (x != 0);
    if (!a850_big && n > 6) return 0;
    a850_n = (int)n;

    cap = 1 << (a850_n + 1);
    a850_pord = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    a850_pdeg = (int *)malloc(sizeof(int) * cap);
    a850_poff = (int *)malloc(sizeof(int) * cap);
    a850_plen = (int *)calloc(cap, sizeof(int));
    a850_pst = (int *)malloc(sizeof(int) * (a850_n + 2));
    a850_np = 0;
    for (d = 1; d <= a850_n; d++) {
        for (f = (uint64_t)1 << d; f < ((uint64_t)1 << (d + 1)); f++) {
            int irr = 1;
            if (!(f & 1)) continue;
            for (g = 2; a850_deg(g) <= d / 2; g++)
                if (a850_mod(f, g) == 0) { irr = 0; break; }
            if (!irr) continue;
            uint64_t ord = 1;
            if (f != 3) {
                uint64_t rr = 2;
                while (rr != 1) { rr = a850_mod(rr << 1, f); ord++; }
            }
            a850_pord[a850_np] = ord;
            a850_pdeg[a850_np] = d;
            a850_np++;
        }
    }

    a850_GLs = 1; a850_GLw = 1;
    a850_accs = 0; a850_accw = 0;
    for (i = 0; i < a850_n; i++) {
        if (a850_big) a850_GLs *= ((seq_t)1 << a850_n) - ((seq_t)1 << i);
        else a850_GLw *= (((unsigned __int128)1) << a850_n) - (((unsigned __int128)1) << i);
    }
    a850_sp = 0;
    a850_poly(0, a850_n);

    if (a850_big) {
        r = a850_accs / (a850_GLs << a850_n);
    } else {
        unsigned __int128 res = a850_accw / (a850_GLw << a850_n);
        r = 0;
        for (i = 127; i >= 0; i--) r = r * 2 + (seq_t)(int)((res >> i) & 1);
    }
    free(a850_pord); free(a850_pdeg); free(a850_poff); free(a850_plen); free(a850_pst);
    return r;
}

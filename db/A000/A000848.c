/*
 * A000848  Number of n-input 3-output switching networks under action of
 *          GL(n,2) on the inputs and complementing group C(3,2) on the
 *          outputs.
 * https://oeis.org/A000848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(8*|GL(n,2)|)) * Sum_{g in GL(n,2)} 8^cyc(g), where
 *            cyc(g) is the number of cycles of g on F_2^n. A nonzero output
 *            complement fixes nothing, because g always fixes the zero
 *            vector. The sum runs over conjugacy classes given by rational
 *            canonical forms: each irreducible p != x gets a part...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   g64, pdeg_, pmod_, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"
#include "../utils/g64.h"
#include "../utils/pdeg_.h"
#include "../utils/pmod_.h"

static int N;
static uint64_t *ir;
static int *irdeg;
static uint64_t *irord;
static int nir;
static int selp[64], selk;
static int parts[64][64], np_[64];
static int use128;
static unsigned __int128 acc128, gl128;
static seq_t accS, glS;
static uint64_t lensA[8192], cntsA[8192], lensB[8192], cntsB[8192];

static void leaf(void) {
    int nm = 1, k, i, j;
    int e = 0;
    uint64_t fac[128];
    int nf = 0;
    uint64_t cyc = 0;
    lensA[0] = 1; cntsA[0] = 1;
    for (k = 0; k < selk; k++) {
        int idx = selp[k], d = irdeg[idx];
        uint64_t ord = irord[idx];
        int M = parts[k][0], np = np_[k];
        uint64_t Lj[65], Cj[65];
        int Kprev = 0;
        int nn = 0;
        long long sl2 = 0, sm2 = 0, smm = 0;
        for (j = 0; j <= M; j++) {
            int K = 0, t = 0;
            for (i = 0; i < np; i++) K += parts[k][i] < j ? parts[k][i] : j;
            if (j == 0) { Cj[j] = 1; Lj[j] = 1; }
            else {
                Cj[j] = ((uint64_t)1 << (d * K)) - ((uint64_t)1 << (d * Kprev));
                while ((1 << t) < j) t++;
                Lj[j] = ord << t;
            }
            Kprev = K;
        }
        for (i = 1; i <= M; i++) {
            long long c = 0, m = 0;
            int q;
            for (q = 0; q < np; q++) { if (parts[k][q] >= i) c++; if (parts[k][q] == i) m++; }
            sl2 += c * c; sm2 += m * m; smm += m * (m - 1) / 2;
            for (q = 1; q <= m; q++) fac[nf++] = ((uint64_t)1 << (d * q)) - 1;
        }
        e += (int)(d * (sl2 - sm2 + smm));
        for (i = 0; i < nm; i++)
            for (j = 0; j <= M; j++) {
                uint64_t a = lensA[i], b = Lj[j];
                uint64_t l = a / g64(a, b) * b, c = cntsA[i] * Cj[j];
                int q;
                for (q = 0; q < nn; q++) if (lensB[q] == l) break;
                if (q == nn) { lensB[nn] = l; cntsB[nn] = 0; nn++; }
                cntsB[q] += c;
            }
        for (i = 0; i < nn; i++) { lensA[i] = lensB[i]; cntsA[i] = cntsB[i]; }
        nm = nn;
    }
    for (i = 0; i < nm; i++) cyc += cntsA[i] / lensA[i];
    if (use128) {
        unsigned __int128 C = (unsigned __int128)1 << e;
        for (i = 0; i < nf; i++) C *= fac[i];
        acc128 += (gl128 / C) * ((unsigned __int128)1 << (3 * cyc));
    } else {
        seq_t C = 1, p = 1;
        C <<= e;
        for (i = 0; i < nf; i++) C *= (seq_t)fac[i];
        p <<= (3 * cyc);
        accS += (glS / C) * p;
    }
}

static void rec_poly(int i, int rem);

static void rec_part(int idx, int rem, int stot, int left, int maxp) {
    int p;
    if (left == 0) {
        selk++;
        rec_poly(idx + 1, rem - irdeg[idx] * stot);
        selk--;
        return;
    }
    for (p = maxp < left ? maxp : left; p >= 1; p--) {
        parts[selk][np_[selk]++] = p;
        rec_part(idx, rem, stot, left - p, p);
        np_[selk]--;
    }
}

static void rec_poly(int i, int rem) {
    int idx, s;
    if (rem == 0) { leaf(); return; }
    for (idx = i; idx < nir; idx++) {
        int d = irdeg[idx];
        if (d > rem) break;
        for (s = 1; s * d <= rem; s++) {
            selp[selk] = idx;
            np_[selk] = 0;
            rec_part(idx, rem, s, s, s);
        }
    }
}

static void gen_irr(int n) {
    uint64_t f, lim = (uint64_t)1 << (n + 1);
    int cap = 1024;
    free(ir); free(irdeg); free(irord);
    ir = (uint64_t *)malloc(cap * sizeof(uint64_t));
    irdeg = (int *)malloc(cap * sizeof(int));
    irord = (uint64_t *)malloc(cap * sizeof(uint64_t));
    nir = 0;
    for (f = 3; f < lim; f += 2) {
        int d = pdeg_(f), ok = 1, q;
        uint64_t cur, k;
        for (q = 0; q < nir && 2 * irdeg[q] <= d; q++)
            if (pmod_(f, ir[q]) == 0) { ok = 0; break; }
        if (!ok) continue;
        if (nir == cap) {
            cap *= 2;
            ir = (uint64_t *)realloc(ir, cap * sizeof(uint64_t));
            irdeg = (int *)realloc(irdeg, cap * sizeof(int));
            irord = (uint64_t *)realloc(irord, cap * sizeof(uint64_t));
        }
        cur = pmod_(2, f); k = 1;
        while (cur != 1) {
            cur <<= 1;
            if ((cur >> d) & 1) cur ^= f;
            k++;
        }
        ir[nir] = f; irdeg[nir] = d; irord[nir] = k; nir++;
    }
}

seq_t a(idx_t n) {
    int k;
    N = (int)n;
    if (N <= 5) {
        use128 = 1;
        gl128 = (unsigned __int128)1 << (N * (N - 1) / 2);
        for (k = 1; k <= N; k++) gl128 *= (((unsigned __int128)1 << k) - 1);
        acc128 = 0;
    } else {
        seq_t t = 1;
        for (k = 0; k < 200; k++) t *= 2;
        if (t == 0) return 0; /* fixed-width: value cannot fit */
        use128 = 0;
        glS = 1;
        glS <<= (N * (N - 1) / 2);
        for (k = 1; k <= N; k++) { seq_t q = 1; q <<= k; glS *= (q - 1); }
        accS = 0;
    }
    gen_irr(N);
    selk = 0;
    rec_poly(0, N);
    if (use128) return u128_to_seq(acc128 / (8 * gl128));
    return accS / (glS * 8);
}

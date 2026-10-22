/*
 * A007127  Definition (1): Number of unlabeled strength-2 Eulerian graphs
 *          with n nodes.
 * https://oeis.org/A007127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{partitions L of n} (1/z(L)) * 2^(-c(L)) * Sum_{e in
 *            {0,1}^c(L)} 3^A(e) (Burnside over S_n, as for switching classes
 *            of signed graphs). c(L) is the number of cycles. A(e) counts the
 *            edge orbits whose character is +1: (k-1)/2 orbits inside each
 *            cycle of length k (floor); one more orbit...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   gcdll, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/gcdll.h"

typedef unsigned long long u64;
typedef unsigned __int128 u128;

static u64 mulm(u64 a, u64 b, u64 p) { return (u64)((u128)a * b % p); }
static u64 powm(u64 a, u64 e, u64 p)
{
    u64 r = 1; a %= p;
    while (e) { if (e & 1) r = mulm(r, a, p); a = mulm(a, a, p); e >>= 1; }
    return r;
}
static bool mr_prime(u64 n)
{
    static const u64 bs[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    u64 d = n - 1; int s = 0, i, r;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) { if (n % bs[i] == 0) return n == bs[i]; }
    while (!(d & 1)) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        u64 x = powm(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) { x = mulm(x, x, n); if (x == n - 1) break; }
        if (r == s) return false;
    }
    return true;
}

static int NN, NP, D;
static size_t PW;
static u64 *PR, *POW3, *INVK, *INVF, *INV2, *TOT, *WT, *SV, *COEF;
static int *LEN, *MUL, *V2, *CLS, *BS;
static long long *WC;

static void rec(int j, int T, long long a)
{
    int q, b, jj;
    if (j == T) {
        for (q = 0; q < NP; q++)
            SV[q] = (SV[q] + mulm(COEF[(size_t)j * NP + q], POW3[(size_t)q * PW + a], PR[q])) % PR[q];
        return;
    }
    {
        int i = CLS[j];
        long long k = LEN[i], m = MUL[i];
        for (b = 0; b <= m; b++) {
            long long add = k * ((long long)b * (b - 1) / 2 + (m - b) * (m - b - 1) / 2) + (m - b) * WC[i];
            for (jj = 0; jj < j; jj++) {
                int i2 = CLS[jj];
                add += gcdll(k, LEN[i2]) * ((long long)b * BS[jj] + (m - b) * (MUL[i2] - BS[jj]));
            }
            BS[j] = b;
            for (q = 0; q < NP; q++) {
                u64 p = PR[q];
                COEF[(size_t)(j + 1) * NP + q] =
                    mulm(mulm(COEF[(size_t)j * NP + q], INVF[(size_t)q * (NN + 1) + b], p),
                         INVF[(size_t)q * (NN + 1) + (m - b)], p);
            }
            rec(j + 1, T, a + add);
        }
    }
}

static void process(void)
{
    long long K = 0;
    int i, l, q, v, vmax = 0, T, mm;
    for (i = 0; i < D; i++) {
        int x = LEN[i], c = 0;
        K += (long long)MUL[i] * ((LEN[i] - 1) / 2);
        while (!(x & 1)) { x >>= 1; c++; }
        V2[i] = c;
        if (c > vmax) vmax = c;
    }
    for (q = 0; q < NP; q++) {
        u64 p = PR[q], w = POW3[(size_t)q * PW + K];
        for (i = 0; i < D; i++) {
            u64 f = mulm(INVK[(size_t)q * (NN + 1) + LEN[i]], INV2[q], p);
            for (mm = 0; mm < MUL[i]; mm++) w = mulm(w, f, p);
        }
        WT[q] = w;
    }
    for (v = 0; v <= vmax; v++) {
        T = 0;
        for (i = 0; i < D; i++) if (V2[i] == v) CLS[T++] = i;
        if (!T) continue;
        for (l = 0; l < T; l++) {
            int ii = CLS[l], j;
            long long w = (LEN[ii] % 2 == 0) ? 1 : 0;
            for (j = 0; j < D; j++)
                if (V2[j] < v) w += (long long)MUL[j] * gcdll(LEN[ii], LEN[j]);
            WC[ii] = w;
        }
        for (q = 0; q < NP; q++) { SV[q] = 0; COEF[q] = 1; }
        rec(0, T, 0);
        for (q = 0; q < NP; q++) WT[q] = mulm(WT[q], SV[q], PR[q]);
    }
    for (q = 0; q < NP; q++) TOT[q] = (TOT[q] + WT[q]) % PR[q];
}

static void gen(int rem, int maxk)
{
    int k, m;
    for (k = (rem < maxk ? rem : maxk); k >= 1; k--) {
        for (m = 1; m * k <= rem; m++) {
            LEN[D] = k; MUL[D] = m; D++;
            if (rem == m * k) process();
            else gen(rem - m * k, k - 1);
            D--;
        }
    }
}

seq_t a(idx_t n)
{
    int i, j, q;
    long long maxA;
    u64 bits, cand;
    u64 *dig;
    seq_t res;

    NN = (int)n;
    maxA = (long long)NN * (NN - 1) / 2;
    bits = (u64)maxA * 1585 / 1000 + 2;
    NP = (int)(bits / 61 + 2);
    if (seq_is_fixed_width() && NP > 3) NP = 3;

    PR = (u64 *)malloc(sizeof(u64) * NP);
    cand = (1ULL << 62) - 1;
    for (q = 0; q < NP; q++) {
        while (!mr_prime(cand)) cand -= 2;
        PR[q] = cand; cand -= 2;
    }
    PW = (size_t)maxA + 1;
    POW3 = (u64 *)malloc(sizeof(u64) * NP * PW);
    INVK = (u64 *)malloc(sizeof(u64) * NP * (NN + 1));
    INVF = (u64 *)malloc(sizeof(u64) * NP * (NN + 1));
    INV2 = (u64 *)malloc(sizeof(u64) * NP);
    TOT = (u64 *)malloc(sizeof(u64) * NP);
    WT = (u64 *)malloc(sizeof(u64) * NP);
    SV = (u64 *)malloc(sizeof(u64) * NP);
    COEF = (u64 *)malloc(sizeof(u64) * NP * (NN + 2));
    LEN = (int *)malloc(sizeof(int) * (NN + 1));
    MUL = (int *)malloc(sizeof(int) * (NN + 1));
    V2 = (int *)malloc(sizeof(int) * (NN + 1));
    CLS = (int *)malloc(sizeof(int) * (NN + 1));
    BS = (int *)malloc(sizeof(int) * (NN + 1));
    WC = (long long *)malloc(sizeof(long long) * (NN + 1));

    for (q = 0; q < NP; q++) {
        u64 p = PR[q], f = 1;
        size_t ii;
        POW3[(size_t)q * PW] = 1;
        for (ii = 1; ii < PW; ii++) POW3[(size_t)q * PW + ii] = mulm(POW3[(size_t)q * PW + ii - 1], 3, p);
        INVK[(size_t)q * (NN + 1)] = 0;
        for (i = 1; i <= NN; i++) INVK[(size_t)q * (NN + 1) + i] = powm((u64)i, p - 2, p);
        INVF[(size_t)q * (NN + 1)] = 1;
        for (i = 1; i <= NN; i++) {
            f = mulm(f, (u64)i, p);
            INVF[(size_t)q * (NN + 1) + i] = powm(f, p - 2, p);
        }
        INV2[q] = powm(2, p - 2, p);
        TOT[q] = 0;
    }

    D = 0;
    gen(NN, NN);

    /* Garner reconstruction */
    dig = (u64 *)malloc(sizeof(u64) * NP);
    for (i = 0; i < NP; i++) {
        u64 p = PR[i], x = TOT[i] % p;
        for (j = 0; j < i; j++) {
            u64 dj = dig[j] % p;
            x = (x + p - dj) % p;
            x = mulm(x, powm(PR[j] % p, p - 2, p), p);
        }
        dig[i] = x;
    }
    res = (seq_t)0;
    for (i = NP - 1; i >= 0; i--) {
        res = res * (seq_t)(idx_t)PR[i] + (seq_t)(idx_t)dig[i];
    }

    free(dig); free(PR); free(POW3); free(INVK); free(INVF); free(INV2);
    free(TOT); free(WT); free(SV); free(COEF); free(LEN); free(MUL);
    free(V2); free(CLS); free(BS); free(WC);
    return res;
}

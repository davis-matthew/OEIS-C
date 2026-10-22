/*
 * A000857  Number of n-input 3-output switching networks under the action of
 *          S(n) on the inputs and S(3) and complementing group C(2,3) on the
 *          outputs.
 * https://oeis.org/A000857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma: a(n) = (1/(48*n!)) * Sum_{sigma in S_n} Sum_{g
 *            in B_3} Prod_{cycles C of sigma acting on {0,1}^n} #Fix(g^|C|),
 *            where B_3 = S(3) x C(2,3) acts on {0,1}^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   g857_gcd, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"
#include "../utils/g857_gcd.h"

static int g857_fl2[48][12]; /* log2 #fixed points of g^k on the cube, -1 if none */
static bool g857_inited = false;
static int g857_N;
static int g857_cnt[70];
static bool g857_use128;
static unsigned __int128 g857_acc128;
static seq_t g857_accS;
static seq_t g857_factS;

static void g857_init(void) {
    int perms[6][3] = {{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
    for (int pi = 0; pi < 6; pi++)
        for (int m = 0; m < 8; m++) {
            int g = pi * 8 + m, img[8];
            for (int v = 0; v < 8; v++) {
                int w = 0;
                for (int j = 0; j < 3; j++)
                    if ((v >> j) & 1) w |= 1 << perms[pi][j];
                img[v] = w ^ m;
            }
            for (int k = 0; k < 12; k++) {
                int c = 0;
                for (int v = 0; v < 8; v++) {
                    int x = v;
                    for (int t = 0; t < k; t++) x = img[x];
                    if (x == v) c++;
                }
                int l = -1;
                if (c) { l = 0; while ((1 << l) < c) l++; }
                g857_fl2[g][k] = l;
            }
        }
    g857_inited = true;
}

static void g857_process(void) {
    int n = g857_N;
    uint64_t Lc = 1;
    for (int i = 1; i <= n; i++)
        if (g857_cnt[i]) Lc = Lc / g857_gcd(Lc, (uint64_t)i) * (uint64_t)i;

    /* divisors of Lc */
    uint64_t pr[32]; int ex[32], np = 0;
    uint64_t r = Lc;
    for (uint64_t p = 2; p * p <= r; p++)
        if (r % p == 0) { pr[np] = p; ex[np] = 0; while (r % p == 0) { r /= p; ex[np]++; } np++; }
    if (r > 1) { pr[np] = r; ex[np] = 1; np++; }
    long long nd = 1;
    for (int i = 0; i < np; i++) nd *= (ex[i] + 1);
    uint64_t *dv = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)nd);
    uint64_t *pts = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)nd);
    long long k = 1;
    dv[0] = 1;
    for (int i = 0; i < np; i++) {
        long long cur = k;
        uint64_t pp = 1;
        for (int e = 1; e <= ex[i]; e++) {
            pp *= pr[i];
            for (long long j = 0; j < cur; j++) dv[k++] = dv[j] * pp;
        }
    }
    for (long long i = 1; i < nd; i++) {
        uint64_t x = dv[i]; long long j = i - 1;
        while (j >= 0 && dv[j] > x) { dv[j + 1] = dv[j]; j--; }
        dv[j + 1] = x;
    }

    uint64_t M[12];
    for (int i = 0; i < 12; i++) M[i] = 0;
    for (long long a = 0; a < nd; a++) {
        uint64_t e = dv[a], c = 0;
        for (int i = 1; i <= n; i++)
            if (g857_cnt[i]) c += (uint64_t)g857_cnt[i] * g857_gcd((uint64_t)i, e);
        uint64_t p = (uint64_t)1 << c;
        for (long long b = 0; b < a; b++)
            if (e % dv[b] == 0) p -= pts[b];
        pts[a] = p;
        M[e % 12] += p / e;
    }
    free(dv);
    free(pts);

    if (g857_use128) {
        uint64_t z = 1, f = 1;
        for (int i = 2; i <= n; i++) f *= (uint64_t)i;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= g857_cnt[i]; j++) z *= (uint64_t)i * (uint64_t)j;
        unsigned __int128 cls = f / z, T = 0;
        for (int g = 0; g < 48; g++) {
            bool ok = true; uint64_t E = 0;
            for (int q = 0; q < 12 && ok; q++)
                if (M[q]) {
                    if (g857_fl2[g][q] < 0) ok = false;
                    else E += M[q] * (uint64_t)g857_fl2[g][q];
                }
            if (ok) T += (unsigned __int128)1 << E;
        }
        g857_acc128 += cls * T;
    } else {
        seq_t z = 1;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= g857_cnt[i]; j++) z *= (seq_t)((idx_t)i * (idx_t)j);
        seq_t cls = g857_factS / z;
        seq_t T = 0;
        for (int g = 0; g < 48; g++) {
            bool ok = true; uint64_t E = 0;
            for (int q = 0; q < 12 && ok; q++)
                if (M[q]) {
                    if (g857_fl2[g][q] < 0) ok = false;
                    else E += M[q] * (uint64_t)g857_fl2[g][q];
                }
            if (ok) { seq_t t = 1; t <<= (idx_t)E; T += t; }
        }
        g857_accS += cls * T;
    }
}

static void g857_gen(int rem, int maxp) {
    if (rem == 0) { g857_process(); return; }
    for (int p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        g857_cnt[p]++;
        g857_gen(rem - p, p);
        g857_cnt[p]--;
    }
}

seq_t a(idx_t n) {
    if (!g857_inited) g857_init();
    g857_N = (int)n;
    for (int i = 0; i < 70; i++) g857_cnt[i] = 0;
    if (n <= 5) {
        g857_use128 = true;
        g857_acc128 = 0;
        g857_gen((int)n, (int)n);
        unsigned __int128 d = 48;
        for (idx_t i = 2; i <= n; i++) d *= (unsigned __int128)i;
        return u128_to_seq(g857_acc128 / d);
    }
    g857_use128 = false;
    g857_accS = 0;
    g857_factS = 1;
    for (idx_t i = 2; i <= n; i++) g857_factS *= (seq_t)i;
    g857_gen((int)n, (int)n);
    return g857_accS / (g857_factS * 48);
}

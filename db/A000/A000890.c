/*
 * A000890  Number of switching networks with S(n) and C(2,2) acting on the
 *          domain and AG(2,3) acting on the range.
 * https://oeis.org/A000890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside / de Bruijn count: a(n) = (1/(2^n*n!*1344)) * Sum_{g in
 *            B_n} Sum_{h in AGL(3,2)} Prod_{cycles c of g on {0,1}^n}
 *            fix(h^|c|), where B_n (S(n) together with input
 *            complementations) acts on the 2^n inputs and AGL(3,2) (order
 *            1344) acts on the 8 output values; summed over conjugacy classes
 *            o...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a890_from_u128, a890_gcd, a890_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a890_from_u128.h"
#include "../utils/a890_gcd.h"
#include "../utils/a890_is_big.h"

/* cycle types of AGL(3,2) on 8 points: (lengths, multiplicities), class sizes */
static const int A890_CNT[8] = {1, 49, 42, 252, 224, 224, 168, 384};
static const int A890_LEN[8][3] = {{1,0,0},{2,0,0},{1,2,0},{4,0,0},{1,3,0},{2,6,0},{1,2,4},{1,7,0}};
static const int A890_MUL[8][3] = {{8,0,0},{4,0,0},{4,2,0},{2,0,0},{2,2,0},{1,1,0},{2,1,1},{1,1,0}};

static int a890_K;
static uint64_t a890_m[70], a890_pa[70], a890_pb[70];
static int a890_bigpath;
static unsigned __int128 a890_acc128;
static uint64_t a890_N64;
static seq_t a890_acc;
static seq_t a890_N;

/* log2 of number of points fixed by h^d for range cycle type t, or -1 if none */
static int a890_lgfix(int t, uint64_t d)
{
    int f = 0, i, l = 0;
    for (i = 0; i < 3; i++)
        if (A890_LEN[t][i] > 0 && d % (uint64_t)A890_LEN[t][i] == 0)
            f += A890_LEN[t][i] * A890_MUL[t][i];
    if (f == 0) return -1;
    while (f > 1) { f >>= 1; l++; }
    return l;
}

/* number of points of {0,1}^n fixed by g^k */
static int64_t a890_fixg(uint64_t k)
{
    int e = 0, i;
    for (i = 0; i < a890_K; i++) {
        uint64_t m = a890_m[i], g = a890_gcd(m, k);
        if (a890_pa[i]) e += (int)(a890_pa[i] * g);
        if (a890_pb[i]) {
            if ((k / g) % 2 == 0 && (m / g) % 2 == 1) e += (int)(a890_pb[i] * g);
            else return 0;
        }
    }
    return (int64_t)1 << e;
}

static void a890_leaf(void)
{
    uint64_t L = 1, q;
    int i, j, t, nd = 0, cap = 64;
    uint64_t *dv;
    int64_t *ex;
    int64_t E[8];
    bool ok[8];

    for (i = 0; i < a890_K; i++) {
        if (a890_pa[i]) L = L / a890_gcd(L, a890_m[i]) * a890_m[i];
        if (a890_pb[i]) L = L / a890_gcd(L, 2 * a890_m[i]) * (2 * a890_m[i]);
    }
    dv = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)cap);
    {
        uint64_t *hi = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)cap);
        int nh = 0;
        for (q = 1; q * q <= L; q++) {
            if (L % q) continue;
            if (nd + 1 >= cap || nh + 1 >= cap) {
                cap *= 2;
                dv = (uint64_t *)realloc(dv, sizeof(uint64_t) * (size_t)cap);
                hi = (uint64_t *)realloc(hi, sizeof(uint64_t) * (size_t)cap);
            }
            dv[nd++] = q;
            if (q != L / q) hi[nh++] = L / q;
        }
        dv = (uint64_t *)realloc(dv, sizeof(uint64_t) * (size_t)(nd + nh + 1));
        for (j = nh - 1; j >= 0; j--) dv[nd++] = hi[j];
        free(hi);
    }
    ex = (int64_t *)malloc(sizeof(int64_t) * (size_t)nd);
    for (i = 0; i < nd; i++) {
        ex[i] = a890_fixg(dv[i]);
        for (j = 0; j < i; j++)
            if (dv[i] % dv[j] == 0) ex[i] -= ex[j];
    }
    for (i = 0; i < nd; i++) ex[i] /= (int64_t)dv[i];   /* number of cycles of length dv[i] */

    for (t = 0; t < 8; t++) {
        E[t] = 0; ok[t] = true;
        for (i = 0; i < nd; i++) {
            int l;
            if (ex[i] == 0) continue;
            l = a890_lgfix(t, dv[i]);
            if (l < 0) { ok[t] = false; break; }
            E[t] += ex[i] * l;
        }
    }
    free(dv);
    free(ex);

    if (!a890_bigpath) {
        uint64_t z = 1, cs;
        unsigned __int128 inner = 0;
        for (i = 0; i < a890_K; i++) {
            uint64_t c;
            for (c = 0; c < a890_pa[i] + a890_pb[i]; c++) z *= 2 * a890_m[i];
            for (c = 2; c <= a890_pa[i]; c++) z *= c;
            for (c = 2; c <= a890_pb[i]; c++) z *= c;
        }
        cs = a890_N64 / z;
        for (t = 0; t < 8; t++)
            if (ok[t]) inner += ((unsigned __int128)A890_CNT[t]) << E[t];
        a890_acc128 += inner * cs;
    } else {
        seq_t z = 1, inner = 0;
        for (i = 0; i < a890_K; i++) {
            uint64_t c;
            for (c = 0; c < a890_pa[i] + a890_pb[i]; c++) z *= (seq_t)(2 * a890_m[i]);
            for (c = 2; c <= a890_pa[i]; c++) z *= (seq_t)c;
            for (c = 2; c <= a890_pb[i]; c++) z *= (seq_t)c;
        }
        for (t = 0; t < 8; t++)
            if (ok[t]) inner += (seq_t)(uint64_t)A890_CNT[t] * ((seq_t)1 << (idx_t)E[t]);
        a890_acc += (a890_N / z) * inner;
    }
}

static void a890_rec(uint64_t m, uint64_t rem)
{
    uint64_t pa, pb;
    if (rem == 0) { a890_leaf(); return; }
    if (m == 0) return;
    for (pa = 0; pa * m <= rem; pa++)
        for (pb = 0; (pa + pb) * m <= rem; pb++) {
            bool push = (pa || pb);
            if (push) {
                a890_m[a890_K] = m; a890_pa[a890_K] = pa; a890_pb[a890_K] = pb; a890_K++;
            }
            a890_rec(m - 1, rem - (pa + pb) * m);
            if (push) a890_K--;
        }
}

seq_t a(idx_t n)
{
    uint64_t i;
    if (n > 60) return 0;
    a890_K = 0;
    if (n <= 5) {
        a890_bigpath = 0;
        a890_acc128 = 0;
        a890_N64 = 1;
        for (i = 1; i <= n; i++) a890_N64 *= 2 * i;
        a890_rec(n, n);
        return a890_from_u128(a890_acc128 / ((unsigned __int128)a890_N64 * 1344U));
    }
    if (!a890_is_big()) return 0;
    a890_bigpath = 1;
    a890_acc = 0;
    a890_N = 1;
    for (i = 1; i <= n; i++) a890_N *= (seq_t)(2 * i);
    a890_rec(n, n);
    return a890_acc / (a890_N * (seq_t)1344);
}

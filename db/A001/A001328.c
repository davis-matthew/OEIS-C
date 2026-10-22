/*
 * A001328  Number of equivalence classes of n-valued Post functions of 2
 *          variables under action of semi-direct product of symmetric groups
 *          S_2 and S(2,n).
 * https://oeis.org/A001328
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma for the wreath product S_n wr S_2, which
 *            permutes each argument and swaps the two, acting on the n x n
 *            argument grid with n colours: a(n) = ( Sum_{L,M |- n} N_L N_M
 *            n^{c(L,M)} + n! Sum_{L |- n} N_L n^{d(L)} ) / (2 n!^2), N_L =
 *            n!/z_L (number of permutations of cycle type L), c(L,...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define A1328_MAXN 40
#define A1328_MAXPT 40000
#define A1328_MAXK 10

static int a1328_np;
static unsigned char a1328_pk[A1328_MAXPT];
static unsigned char a1328_pp[A1328_MAXPT][A1328_MAXK];
static unsigned char a1328_pm[A1328_MAXPT][A1328_MAXK];
static seq_t a1328_sz[A1328_MAXPT];
static seq_t a1328_pw[A1328_MAXN * A1328_MAXN + 1];
static int a1328_G[A1328_MAXN + 1][A1328_MAXN + 1];

static void a1328_rec(int rem, int maxp, int k, unsigned char *cp, unsigned char *cm)
{
    int p, m, t;
    if (rem == 0) {
        if (a1328_np < A1328_MAXPT) {
            a1328_pk[a1328_np] = (unsigned char)k;
            for (t = 0; t < k; t++) {
                a1328_pp[a1328_np][t] = cp[t];
                a1328_pm[a1328_np][t] = cm[t];
            }
            a1328_np++;
        }
        return;
    }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--)
        for (m = 1; m * p <= rem; m++) {
            cp[k] = (unsigned char)p;
            cm[k] = (unsigned char)m;
            a1328_rec(rem - p * m, p - 1, k + 1, cp, cm);
        }
}

static int a1328_cyc(int i, int j)
{
    int a, b, s = 0;
    for (a = 0; a < a1328_pk[i]; a++)
        for (b = 0; b < a1328_pk[j]; b++)
            s += a1328_pm[i][a] * a1328_pm[j][b] *
                 a1328_G[a1328_pp[i][a]][a1328_pp[j][b]];
    return s;
}

static int a1328_cswap(int i)
{
    int a, b, s = 0;
    for (a = 0; a < a1328_pk[i]; a++) {
        int p = a1328_pp[i][a], m = a1328_pm[i][a];
        s += m * ((p + 1) / 2) + (m * (m - 1) / 2) * p;
        for (b = a + 1; b < a1328_pk[i]; b++)
            s += m * a1328_pm[i][b] * a1328_G[p][a1328_pp[i][b]];
    }
    return s;
}

seq_t a(idx_t n)
{
    int N, i, j, k, w, big, x0, y0;
    unsigned char cp[A1328_MAXK], cm[A1328_MAXK];
    uint64_t f64 = 1;
    seq_t fact = 1, T = 0, B = 0, x = 1;

    if (n < 1 || n > A1328_MAXN) return 0;
    N = (int)n;

    w = 0;
    while (w < 256 && x != 0) { x <<= 1; w++; }
    big = (w >= 256);
    if (!big && N > 20) return 0;

    for (x0 = 0; x0 <= A1328_MAXN; x0++)
        for (y0 = 0; y0 <= A1328_MAXN; y0++) {
            int u = x0, v = y0;
            while (v) { int t = u % v; u = v; v = t; }
            a1328_G[x0][y0] = u;
        }

    a1328_np = 0;
    a1328_rec(N, N, 0, cp, cm);

    for (k = 1; k <= N; k++) {
        fact *= (seq_t)k;
        if (!big) f64 *= (uint64_t)k;
    }

    for (i = 0; i < a1328_np; i++) {
        if (big) {
            seq_t z = 1;
            for (k = 0; k < a1328_pk[i]; k++) {
                int t;
                for (t = 1; t <= a1328_pm[i][k]; t++)
                    z *= (seq_t)(a1328_pp[i][k] * t);
            }
            a1328_sz[i] = fact / z;
        } else {
            uint64_t z = 1;
            for (k = 0; k < a1328_pk[i]; k++) {
                int t;
                for (t = 1; t <= a1328_pm[i][k]; t++)
                    z *= (uint64_t)(a1328_pp[i][k] * t);
            }
            a1328_sz[i] = (seq_t)(f64 / z);
        }
    }

    a1328_pw[0] = 1;
    for (k = 1; k <= N * N; k++)
        a1328_pw[k] = (seq_t)(a1328_pw[k - 1] * (seq_t)n);

    for (i = 0; i < a1328_np; i++) {
        seq_t inner = 0;
        for (j = i + 1; j < a1328_np; j++)
            inner += (seq_t)(a1328_sz[j] * a1328_pw[a1328_cyc(i, j)]);
        T += (seq_t)(a1328_sz[i] *
             (seq_t)((seq_t)(a1328_sz[i] * a1328_pw[a1328_cyc(i, i)]) + (seq_t)(inner * 2)));
        B += (seq_t)(a1328_sz[i] * a1328_pw[a1328_cswap(i)]);
    }
    T += (seq_t)(fact * B);

    if (big)
        return T / ((seq_t)2 * fact * fact);

    {
        int v = 0, e, it;
        uint64_t o = f64;
        seq_t q, inv, r, mask;
        while (!(o & 1)) { o >>= 1; v++; }
        e = 1 + 2 * v;
        if (e >= w) return 0;
        q = (seq_t)((seq_t)o * (seq_t)o);
        inv = q;
        for (it = 0; it < 8; it++)
            inv = (seq_t)(inv * (seq_t)(2 - q * inv));
        r = (seq_t)((seq_t)(T >> e) * inv);
        mask = (seq_t)(((seq_t)1 << (w - e)) - 1);
        return (seq_t)(r & mask);
    }
}

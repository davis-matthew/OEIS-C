/*
 * A000273  Number of unlabeled simple digraphs with n nodes.
 * https://oeis.org/A000273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, nice
 * Formula:   a(n) = Sum_{partitions L of n} 2^c(L) / z(L), where c(L) =
 *            Sum_{i,j} gcd(L_i,L_j) - n (cycles of the induced permutation on
 *            ordered pairs of distinct nodes) and z(L) = Prod_k k^m_k * m_k!
 *            (Polya / Burnside, Harary-Palmer). The power of 2 in z(L) always
 *            divides 2^c(L); the odd part is handled by e...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   a273_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a273_gcd.h"

typedef unsigned __int128 a273_u128;

static int a273_fixed;
static int *a273_P;
static int a273_n;
static int *a273_G;
static seq_t a273_SB;
static a273_u128 a273_SF;

static int a273_g(int x, int y)
{
    if (a273_G) return a273_G[x * (a273_n + 1) + y];
    return a273_gcd(x, y);
}

static a273_u128 a273_inv(a273_u128 d)
{
    a273_u128 x = d;
    int i;
    for (i = 0; i < 7; i++) x *= (a273_u128)2 - d * x;
    return x;
}

static void a273_dfs(int rem, int maxk, int np, long long c, long long v,
                     int lastmult, seq_t qb, a273_u128 qf)
{
    int k;
    if (rem == 0) {
        long long e = c - v;
        if (a273_fixed) {
            if (e < 128) a273_SF += qf << (int)e;
        } else {
            a273_SB += qb << (int)e;
        }
        return;
    }
    for (k = (rem < maxk ? rem : maxk); k >= 1; k--) {
        int m = (np > 0 && a273_P[np - 1] == k) ? lastmult + 1 : 1;
        long long dc = k - 1;
        int j, dv = 0;
        long long d, od;
        for (j = 0; j < np; j++) dc += 2 * a273_g(a273_P[j], k);
        d = (long long)k * m;
        od = d;
        while ((od & 1) == 0) { od >>= 1; dv++; }
        a273_P[np] = k;
        if (a273_fixed)
            a273_dfs(rem - k, k, np + 1, c + dc, v + dv, m, qb,
                     qf * a273_inv((a273_u128)od));
        else
            a273_dfs(rem - k, k, np + 1, c + dc, v + dv, m,
                     qb / (seq_t)od, qf);
    }
}

static seq_t a273_from_u128(a273_u128 u)
{
    seq_t r = (seq_t)(uint64_t)(u >> 64);
    int i;
    for (i = 0; i < 8; i++) r *= 256;
    r += (seq_t)(uint64_t)u;
    return r;
}

seq_t a(idx_t n)
{
    int N = (int)n, i;
    seq_t x = 1, O = 1;
    for (i = 0; i < 130; i++) x = x * 2;
    a273_fixed = (x == 0);
    a273_n = N;
    a273_P = (int *)malloc(sizeof(int) * (size_t)(N + 1));
    a273_G = NULL;
    if (N <= 512) {
        int r, s;
        a273_G = (int *)malloc(sizeof(int) * (size_t)(N + 1) * (size_t)(N + 1));
        for (r = 0; r <= N; r++)
            for (s = 0; s <= N; s++)
                a273_G[r * (N + 1) + s] = a273_gcd(r, s);
    }
    if (!a273_fixed) {
        for (i = 2; i <= N; i++) {
            int o = i;
            while ((o & 1) == 0) o >>= 1;
            O *= (seq_t)o;
        }
    }
    a273_SB = 0;
    a273_SF = 0;
    a273_dfs(N, N, 0, 0, 0, 0, O, (a273_u128)1);
    free(a273_P);
    a273_P = NULL;
    if (a273_G) { free(a273_G); a273_G = NULL; }
    if (a273_fixed) return a273_from_u128(a273_SF);
    return a273_SB / O;
}

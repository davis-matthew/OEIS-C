/*
 * A007130  Number of unlabeled rooted strength 3 Eulerian graphs with n
 *          nodes.
 * https://oeis.org/A007130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   g(n) = number of Eulerian multigraphs (edge multiplicity <= 3,
 *            no loops) on n unlabeled nodes, by Burnside over cycle types p =
 *            (L_1,...,L_c) of S_n: g(n) = Sum_p 4^E(p) / (2^r(p) * z(p)),
 *            E(p) = Sum_i floor(L_i/2) + Sum_{i<j} gcd(L_i,L_j), r(p) = c -
 *            [p has an odd cycle], z(p) = Prod L^m_L * m_L...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   gcd_i32, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd_i32.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

typedef unsigned __int128 u128;

static int pl[256], pm[256];
static int big_mode;
static u128 acc128;
static seq_t *bkt;

static u128 inv128(u128 o)
{
    u128 y = o;
    int i;
    for (i = 0; i < 7; i++) y *= (u128)2 - o * y;
    return y;
}

static void rec(int rem, int maxL, int depth, long E, int cyc, int anyodd,
                int s, u128 o, seq_t cnt)
{
    int L, m, j;
    if (rem == 0) {
        long x = 2 * E - (anyodd ? cyc - 1 : cyc);
        if (big_mode) bkt[x] += cnt;
        else {
            long sh = x - s;
            if (sh < 128) acc128 += inv128(o) << sh;
        }
        return;
    }
    for (L = maxL < rem ? maxL : rem; L >= 1; L--) {
        long cross = 0;
        int vL = 0, oL = L, s2 = s;
        u128 o2 = o;
        seq_t c2 = cnt;
        for (j = 0; j < depth; j++) cross += (long)pm[j] * gcd_i32(L, pl[j]);
        while (!(oL & 1)) { oL >>= 1; vL++; }
        for (m = 1; m * L <= rem; m++) {
            int vm = 0, om = m;
            long dE;
            while (!(om & 1)) { om >>= 1; vm++; }
            s2 += vL + vm;
            o2 *= (u128)oL * (u128)om;
            if (big_mode) { c2 /= (idx_t)L; c2 /= (idx_t)m; }
            dE = (long)m * (L / 2) + (long)m * (m - 1) / 2 * L + (long)m * cross;
            pl[depth] = L; pm[depth] = m;
            rec(rem - m * L, L - 1, depth + 1, E + dE, cyc + m, anyodd || (L & 1),
                s2, o2, c2);
        }
    }
}

seq_t a(idx_t n)
{
    int N = (int)n, m, k, d;
    if (n < 1) return 0;
    if (seq_is_fixed_width()) {
        static u128 g[21], b[21], c[21];
        static int done = 0;
        if (n > 20) return 0;
        big_mode = 0;
        for (m = done + 1; m <= N; m++) {
            u128 t;
            int v = 0, o = m;
            acc128 = 0;
            rec(m, m, 0, 0, 0, 0, 0, (u128)1, (seq_t)0);
            g[m] = acc128;
            t = (u128)m * g[m];
            for (k = 1; k < m; k++) t -= b[k] * g[m - k];
            b[m] = t;
            for (d = 1; d < m; d++) if (m % d == 0) t -= (u128)d * c[d];
            while (!(o & 1)) { o >>= 1; v++; }
            c[m] = (t >> v) * inv128((u128)o);
            done = m;
        }
        return u128_to_seq(c[N]);
    } else {
        static seq_t *G = 0, *B = 0, *C = 0;
        static idx_t len = 0;
        if (n + 1 > len) {
            idx_t old = len;
            G = grow_zero(G, old, n + 1);
            B = grow_zero(B, old, n + 1);
            C = grow_zero(C, old, n + 1);
            len = n + 1;
            big_mode = 1;
            for (m = old < 1 ? 1 : (int)old; m <= N; m++) {
                seq_t f = 1, tot = 0, t, sub = 0;
                idx_t X = (idx_t)m * (idx_t)m + 1, x;
                for (k = 2; k <= m; k++) f *= (idx_t)k;
                bkt = alloc_zero(X);
                rec(m, m, 0, 0, 0, 0, 0, (u128)1, f);
                for (x = 0; x < X; x++)
                    if (bkt[x] != 0) tot += bkt[x] * pow2(x);
                release(bkt, X);
                G[m] = tot / f;
                t = G[m] * (idx_t)m;
                for (k = 1; k < m; k++) sub += B[k] * G[m - k];
                t -= sub;
                B[m] = t;
                sub = 0;
                for (d = 1; d < m; d++) if (m % d == 0) sub += C[d] * (idx_t)d;
                C[m] = (t - sub) / (idx_t)m;
            }
        }
        return C[N];
    }
}

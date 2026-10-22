/*
 * A000887  Number of switching networks with S(n,2) acting on the domain and
 *          AG(3,2) acting on the range where S(n,k) is the symmetric group
 *          acting on k variables.
 * https://oeis.org/A000887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n!*1344)) * Sum_{sigma in S_n} Sum_{g in AG(3,2)}
 *            Prod_{cycles C of sigma on {0,1}^n} fix(g^{|C|}), by Burnside's
 *            lemma: the number of orbits of maps {0,1}^n -> F_2^3 when S_n
 *            permutes the input coordinates and AG(3,2) acts on the outputs.
 *            The sum runs over the cycle types of S_n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a887_gcd, a887_pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a887_gcd.h"
#include "../utils/a887_pw2.h"

static int G_ncls = 0;
static int G_lg[128][85];
static uint64_t G_cnt[128];
static int G_init = 0;

static int B_mode;
static unsigned __int128 S128;
static unsigned __int128 F128;
static seq_t *Sbig;
static seq_t *Fbig;

static void a887_init_group(void)
{
    int A, b, x, k, c;
    if (G_init) return;
    for (A = 0; A < 512; A++) {
        int col[3], img[8], seen = 0, ok = 1;
        col[0] = A & 7; col[1] = (A >> 3) & 7; col[2] = (A >> 6) & 7;
        for (x = 0; x < 8; x++) {
            int v = 0;
            if (x & 1) v ^= col[0];
            if (x & 2) v ^= col[1];
            if (x & 4) v ^= col[2];
            img[x] = v;
            if (seen & (1 << v)) ok = 0;
            seen |= 1 << v;
        }
        if (!ok) continue;
        for (b = 0; b < 8; b++) {
            int p[8], q[8], lg[85];
            for (x = 0; x < 8; x++) { p[x] = img[x] ^ b; q[x] = x; }
            lg[0] = 3;
            for (k = 1; k <= 84; k++) {
                int f = 0, l;
                for (x = 0; x < 8; x++) q[x] = p[q[x]];
                for (x = 0; x < 8; x++) if (q[x] == x) f++;
                if (f == 0) l = -1;
                else { l = 0; while ((1 << l) < f) l++; }
                lg[k] = l;
            }
            for (c = 0; c < G_ncls; c++) {
                int same = 1;
                for (k = 1; k <= 84; k++) if (G_lg[c][k] != lg[k]) { same = 0; break; }
                if (same) break;
            }
            if (c == G_ncls) {
                for (k = 0; k <= 84; k++) G_lg[c][k] = lg[k];
                G_cnt[c] = 0;
                G_ncls++;
            }
            G_cnt[c]++;
        }
    }
    G_init = 1;
}

static void a887_leaf(const int *parts, int k)
{
    uint64_t L = 1, d, e;
    uint64_t *cyc;
    int i, cl;
    for (i = 0; i < k; i++) L = L / a887_gcd(L, (uint64_t)parts[i]) * (uint64_t)parts[i];
    cyc = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    for (d = 1; d <= L; d++) {
        uint64_t c = 0, s;
        if (L % d) continue;
        for (i = 0; i < k; i++) c += a887_gcd((uint64_t)parts[i], d);
        s = (uint64_t)1 << c;
        for (e = 1; e < d; e++) if (d % e == 0) s -= e * cyc[e];
        cyc[d] = s / d;
    }
    if (!B_mode) {
        unsigned __int128 sub = 0, z = 1;
        for (cl = 0; cl < G_ncls; cl++) {
            uint64_t E = 0; int ok = 1;
            for (d = 1; d <= L; d++) {
                int l;
                if (L % d || cyc[d] == 0) continue;
                l = G_lg[cl][(d - 1) % 84 + 1];
                if (l < 0) { ok = 0; break; }
                E += cyc[d] * (uint64_t)l;
            }
            if (ok) sub += ((unsigned __int128)G_cnt[cl]) << E;
        }
        i = 0;
        while (i < k) {
            int j = i, m;
            while (j < k && parts[j] == parts[i]) j++;
            for (m = 1; m <= j - i; m++) z *= (unsigned __int128)parts[i] * (unsigned __int128)m;
            i = j;
        }
        S128 += F128 / z * sub;
    } else {
        seq_t sub = 0, z = 1;
        for (cl = 0; cl < G_ncls; cl++) {
            uint64_t E = 0; int ok = 1;
            for (d = 1; d <= L; d++) {
                int l;
                if (L % d || cyc[d] == 0) continue;
                l = G_lg[cl][(d - 1) % 84 + 1];
                if (l < 0) { ok = 0; break; }
                E += cyc[d] * (uint64_t)l;
            }
            if (ok) sub += (seq_t)G_cnt[cl] * a887_pw2(E);
        }
        i = 0;
        while (i < k) {
            int j = i, m;
            while (j < k && parts[j] == parts[i]) j++;
            for (m = 1; m <= j - i; m++) { z *= (seq_t)parts[i]; z *= (seq_t)m; }
            i = j;
        }
        *Sbig += *Fbig / z * sub;
    }
    free(cyc);
}

static void a887_rec(int rem, int maxp, int *parts, int k)
{
    int p;
    if (rem == 0) { a887_leaf(parts, k); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        parts[k] = p;
        a887_rec(rem - p, p, parts, k + 1);
    }
}

seq_t a(idx_t n)
{
    int parts[64];
    idx_t i;
    if (n < 1 || n > 62) return 0;
    a887_init_group();
    if (n <= 5) {
        unsigned __int128 res;
        seq_t r = 0;
        int j;
        B_mode = 0;
        S128 = 0;
        F128 = 1;
        for (i = 2; i <= n; i++) F128 *= (unsigned __int128)i;
        a887_rec((int)n, (int)n, parts, 0);
        res = S128 / (F128 * 1344);
        for (j = 7; j >= 0; j--)
            r = r * 65536 + (seq_t)(unsigned)((res >> (16 * j)) & 0xFFFF);
        return r;
    } else {
        seq_t S = 0, F = 1;
        B_mode = 1;
        for (i = 2; i <= n; i++) F *= (seq_t)i;
        Sbig = &S;
        Fbig = &F;
        a887_rec((int)n, (int)n, parts, 0);
        return S / (F * 1344);
    }
}

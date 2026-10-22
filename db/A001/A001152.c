/*
 * A001152  Number of n-input 3-output switching networks with GL(n,2) acting
 *          on the input and S(3) and C(2,3) acting on the output.
 * https://oeis.org/A001152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(48*|GL(n,2)|)) * Sum_{g in GL(n,2)} Sum_{h in S(3) x
 *            C(2)^3} Prod_{cycles of g on GF(2)^n, length k} fix(h^k)
 *            (Burnside's lemma). Classes of GL(n,2) are enumerated by
 *            rational canonical form: one partition per irreducible f != x,
 *            with centralizer size Prod_f Q^(Sum lambda'_i^2) Prod_i...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a1152_gcd, a1152_totient, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a1152_gcd.h"
#include "../utils/a1152_totient.h"

typedef struct { int d; uint64_t o; } a1152_irr_t;

static a1152_irr_t *a1152_irr = NULL;
static int a1152_nirr, a1152_cap;
static int a1152_bd[128], a1152_be[128];
static uint64_t a1152_bo[128];
static int a1152_nb;
static int a1152_cu[128];
static int a1152_ncu;
static long long a1152_cT;
static int a1152_nrow = 0;
static int a1152_rmult[48];
static int a1152_rlog[48][12];
static int a1152_big;
static unsigned __int128 a1152_S128, a1152_GL128;
static seq_t a1152_S, a1152_GL;

static void a1152_build_rows(void) {
    static const int perms[6][3] = {{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
    int p, c, y, r, i, t;
    a1152_nrow = 0;
    for (p = 0; p < 6; p++) for (c = 0; c < 8; c++) {
        int img[8], row[12], found = -1;
        for (y = 0; y < 8; y++) {
            int z = 0;
            for (i = 0; i < 3; i++) if ((y >> i) & 1) z |= 1 << perms[p][i];
            img[y] = z ^ c;
        }
        for (r = 0; r < 12; r++) {
            int cnt = 0, lg = -1;
            for (y = 0; y < 8; y++) {
                int z = y;
                for (t = 0; t < r; t++) z = img[z];
                if (z == y) cnt++;
            }
            if (cnt) { lg = 0; while ((1 << lg) < cnt) lg++; }
            row[r] = lg;
        }
        for (i = 0; i < a1152_nrow && found < 0; i++)
            if (memcmp(a1152_rlog[i], row, sizeof(row)) == 0) found = i;
        if (found >= 0) a1152_rmult[found]++;
        else {
            memcpy(a1152_rlog[a1152_nrow], row, sizeof(row));
            a1152_rmult[a1152_nrow++] = 1;
        }
    }
}

static void a1152_check(int d, uint64_t o) {
    int k;
    uint64_t cnt, i;
    if (o == 1) k = 1;
    else {
        uint64_t t = 2 % o;
        k = 1;
        while (t != 1) { t = t * 2 % o; k++; }
    }
    if (k != d) return;
    cnt = a1152_totient(o) / (uint64_t)d;
    for (i = 0; i < cnt; i++) {
        if (a1152_nirr == a1152_cap) {
            a1152_cap = a1152_cap ? 2 * a1152_cap : 64;
            a1152_irr = (a1152_irr_t *)realloc(a1152_irr, (size_t)a1152_cap * sizeof(a1152_irr_t));
        }
        a1152_irr[a1152_nirr].d = d;
        a1152_irr[a1152_nirr].o = o;
        a1152_nirr++;
    }
}

static int a1152_D(uint64_t m, int s) {
    int i, D = 0, p2 = 1 << s;
    for (i = 0; i < a1152_nb; i++)
        if (m % a1152_bo[i] == 0)
            D += a1152_bd[i] * (a1152_be[i] < p2 ? a1152_be[i] : p2);
    return D;
}

static void a1152_process(void) {
    uint64_t M = 1, x, p, m;
    uint64_t pr[64], E[12];
    int pe[64], ex[64], qs[64];
    int maxe = 0, s0 = 0, np = 0, i, t, s, r, row;
    for (r = 0; r < 12; r++) E[r] = 0;
    for (i = 0; i < a1152_nb; i++) {
        M = M / a1152_gcd(M, a1152_bo[i]) * a1152_bo[i];
        if (a1152_be[i] > maxe) maxe = a1152_be[i];
    }
    while ((1 << s0) < maxe) s0++;
    x = M;
    for (p = 3; p * p <= x; p += 2)
        if (x % p == 0) {
            pr[np] = p; pe[np] = 0;
            while (x % p == 0) { x /= p; pe[np]++; }
            np++;
        }
    if (x > 1) { pr[np] = x; pe[np] = 1; np++; }
    for (i = 0; i < np; i++) ex[i] = 0;
    for (;;) {
        int nq = 0;
        m = 1;
        for (i = 0; i < np; i++) {
            for (t = 0; t < ex[i]; t++) m *= pr[i];
            if (ex[i] > 0) qs[nq++] = i;
        }
        for (s = 0; s <= s0; s++) {
            int nbits = nq + (s > 0 ? 1 : 0);
            long long P = 0;
            uint64_t mask, k;
            for (mask = 0; mask < ((uint64_t)1 << nbits); mask++) {
                uint64_t mm = m;
                int ss = s, bits = 0;
                long long f;
                for (i = 0; i < nq; i++)
                    if ((mask >> i) & 1) { mm /= pr[qs[i]]; bits++; }
                if (s > 0 && ((mask >> nq) & 1)) { ss--; bits++; }
                f = 1LL << a1152_D(mm, ss);
                P += (bits & 1) ? -f : f;
            }
            k = ((uint64_t)1 << s) * m;
            E[k % 12] += (uint64_t)P / k;
        }
        for (i = 0; i < np; i++) {
            if (ex[i] < pe[i]) { ex[i]++; break; }
            ex[i] = 0;
        }
        if (i == np) break;
    }
    if (a1152_big) {
        seq_t F = 0, C;
        for (row = 0; row < a1152_nrow; row++) {
            uint64_t X = 0;
            int zero = 0;
            for (r = 0; r < 12; r++) {
                if (E[r] == 0) continue;
                if (a1152_rlog[row][r] < 0) { zero = 1; break; }
                X += (uint64_t)a1152_rlog[row][r] * E[r];
            }
            if (!zero) F += ((seq_t)1 << (idx_t)X) * (seq_t)a1152_rmult[row];
        }
        C = (seq_t)1 << (idx_t)a1152_cT;
        for (i = 0; i < a1152_ncu; i++)
            C *= ((seq_t)1 << (idx_t)a1152_cu[i]) - 1;
        a1152_S += a1152_GL / C * F;
    } else {
        unsigned __int128 F = 0, C;
        for (row = 0; row < a1152_nrow; row++) {
            uint64_t X = 0;
            int zero = 0;
            for (r = 0; r < 12; r++) {
                if (E[r] == 0) continue;
                if (a1152_rlog[row][r] < 0) { zero = 1; break; }
                X += (uint64_t)a1152_rlog[row][r] * E[r];
            }
            if (!zero) F += ((unsigned __int128)1 << X) * (unsigned __int128)a1152_rmult[row];
        }
        C = (unsigned __int128)1 << a1152_cT;
        for (i = 0; i < a1152_ncu; i++)
            C *= ((unsigned __int128)1 << a1152_cu[i]) - 1;
        a1152_S128 += a1152_GL128 / C * F;
    }
}

static void a1152_rec(int j0, int rem);

static void a1152_part(int j, int left, int maxp, int *parts, int np, int remAfter) {
    int p;
    if (left == 0) {
        int d = a1152_irr[j].d;
        uint64_t o = a1152_irr[j].o;
        int sb = a1152_nb, sc = a1152_ncu;
        long long sT = a1152_cT, s2 = 0;
        int i, t;
        for (i = 1; i <= parts[0]; i++) {
            long long c = 0;
            for (t = 0; t < np; t++) if (parts[t] >= i) c++;
            s2 += c * c;
        }
        t = 0;
        while (t < np) {
            int u = t, mm;
            while (u < np && parts[u] == parts[t]) u++;
            mm = u - t;
            s2 -= (long long)mm * (mm + 1) / 2;
            for (i = 1; i <= mm; i++) a1152_cu[a1152_ncu++] = d * i;
            t = u;
        }
        a1152_cT += (long long)d * s2;
        for (t = 0; t < np; t++) {
            a1152_bd[a1152_nb] = d;
            a1152_bo[a1152_nb] = o;
            a1152_be[a1152_nb] = parts[t];
            a1152_nb++;
        }
        a1152_rec(j + 1, remAfter);
        a1152_nb = sb;
        a1152_ncu = sc;
        a1152_cT = sT;
        return;
    }
    for (p = left < maxp ? left : maxp; p >= 1; p--) {
        parts[np] = p;
        a1152_part(j, left - p, p, parts, np + 1, remAfter);
    }
}

static void a1152_rec(int j0, int rem) {
    int j, k;
    if (rem == 0) { a1152_process(); return; }
    for (j = j0; j < a1152_nirr; j++) {
        int d = a1152_irr[j].d;
        if (d > rem) break;
        for (k = 1; k * d <= rem; k++) {
            int parts[128];
            a1152_part(j, k, k, parts, 0, rem - k * d);
        }
    }
}

seq_t a(idx_t n) {
    int i, d, nn = (int)n;
    seq_t x = 1;
    for (i = 0; i < 130; i++) x *= 2;
    if (x == 0 && n > 5) return 0;
    if (a1152_nrow == 0) a1152_build_rows();
    a1152_nirr = 0;
    for (d = 1; d <= nn; d++) {
        uint64_t M = ((uint64_t)1 << d) - 1, q;
        for (q = 1; q * q <= M; q++)
            if (M % q == 0) {
                a1152_check(d, q);
                if (q != M / q) a1152_check(d, M / q);
            }
    }
    a1152_nb = 0;
    a1152_ncu = 0;
    a1152_cT = 0;
    if (n <= 5) {
        a1152_big = 0;
        a1152_S128 = 0;
        a1152_GL128 = 1;
        for (i = 0; i < nn; i++)
            a1152_GL128 *= ((unsigned __int128)1 << nn) - ((unsigned __int128)1 << i);
        a1152_rec(0, nn);
        return u128_to_seq(a1152_S128 / (a1152_GL128 * 48));
    }
    a1152_big = 1;
    a1152_S = 0;
    a1152_GL = 1;
    for (i = 0; i < nn; i++)
        a1152_GL *= ((seq_t)1 << n) - ((seq_t)1 << (idx_t)i);
    a1152_rec(0, nn);
    return a1152_S / (a1152_GL * 48);
}

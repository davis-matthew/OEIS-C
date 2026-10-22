/*
 * A003855  Degrees of irreducible representations of Mathieu group M_11.
 * https://oeis.org/A003855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n-th smallest irreducible character degree of M_11,
 *            found by building M_11 = <(1,...,11), (3,7,11,8)(4,10,5,6)>,
 *            computing its class-algebra structure constants, and running
 *            Dixon's eigenvector method mod p = 1321
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define NPTS 11
#define MAXG 8192
#define HSZ 16384
#define MAXC 32
#define PR 1321LL

static unsigned char Gel[MAXG][NPTS];
static int Gn;
static int htab[HSZ];
static int cls[MAXG];
static int degs[MAXC];
static int nclasses;
static bool ready = false;

static uint64_t pkey(const unsigned char *p) {
    uint64_t k = 0;
    int i;
    for (i = 0; i < NPTS; i++) k = k * 16 + p[i];
    return k;
}

static unsigned phash(uint64_t k) {
    k ^= k >> 29;
    k *= 0x9E3779B97F4A7C15ULL;
    return (unsigned)(k >> 40) & (HSZ - 1);
}

static int pfind(const unsigned char *p) {
    uint64_t k = pkey(p);
    unsigned h = phash(k);
    while (htab[h]) {
        if (pkey(Gel[htab[h] - 1]) == k) return htab[h] - 1;
        h = (h + 1) & (HSZ - 1);
    }
    return -1;
}

static int padd(const unsigned char *p) {
    uint64_t k = pkey(p);
    unsigned h = phash(k);
    while (htab[h]) {
        if (pkey(Gel[htab[h] - 1]) == k) return htab[h] - 1;
        h = (h + 1) & (HSZ - 1);
    }
    if (Gn >= MAXG) return -1;
    memcpy(Gel[Gn], p, NPTS);
    htab[h] = Gn + 1;
    return Gn++;
}

static void pmul(const unsigned char *a, const unsigned char *b, unsigned char *out) {
    int i;
    for (i = 0; i < NPTS; i++) out[i] = b[a[i]];
}

static void pinv(const unsigned char *a, unsigned char *out) {
    int i;
    for (i = 0; i < NPTS; i++) out[a[i]] = (unsigned char)i;
}

static long long mpow(long long b, long long e) {
    long long r = 1;
    b %= PR;
    if (b < 0) b += PR;
    while (e > 0) {
        if (e & 1) r = r * b % PR;
        b = b * b % PR;
        e >>= 1;
    }
    return r;
}

static long long minv(long long x) { return mpow(x, PR - 2); }

/* returns nullity of A (r x r, destroyed); if nullity is 1, fills v */
static int nullvec(long long A[MAXC][MAXC], int r, long long *v) {
    int pivcol[MAXC];
    bool ispiv[MAXC];
    int row = 0, col, i, j, f;
    for (i = 0; i < r; i++) ispiv[i] = false;
    for (col = 0; col < r && row < r; col++) {
        int sel = -1;
        long long iv;
        for (i = row; i < r; i++)
            if (A[i][col] != 0) { sel = i; break; }
        if (sel < 0) continue;
        if (sel != row)
            for (j = 0; j < r; j++) {
                long long t = A[sel][j]; A[sel][j] = A[row][j]; A[row][j] = t;
            }
        iv = minv(A[row][col]);
        for (j = 0; j < r; j++) A[row][j] = A[row][j] * iv % PR;
        for (i = 0; i < r; i++) {
            long long fct;
            if (i == row || A[i][col] == 0) continue;
            fct = A[i][col];
            for (j = 0; j < r; j++) {
                A[i][j] = (A[i][j] - fct * A[row][j]) % PR;
                if (A[i][j] < 0) A[i][j] += PR;
            }
        }
        pivcol[row] = col;
        ispiv[col] = true;
        row++;
    }
    if (r - row != 1) return r - row;
    f = -1;
    for (i = 0; i < r; i++)
        if (!ispiv[i]) { f = i; break; }
    for (i = 0; i < r; i++) v[i] = 0;
    v[f] = 1;
    for (i = 0; i < row; i++) v[pivcol[i]] = (PR - A[i][f]) % PR;
    return 1;
}

static void compute(void) {
    /* (3,7,11,8)(4,10,5,6) in 0-based points: (2,6,10,7)(3,9,4,5) */
    static const unsigned char g2[NPTS] = {0, 1, 6, 9, 5, 3, 10, 2, 8, 4, 7};
    unsigned char gens[2][NPTS], ginv[2][NPTS], id[NPTS], tmp[NPTS], tmp2[NPTS];
    static long long cc[MAXC][MAXC][MAXC];
    static long long M[MAXC][MAXC], A[MAXC][MAXC];
    static int stackv[MAXG];
    long long csz[MAXC];
    int rep[MAXC], invc[MAXC];
    int i, j, k, l, q, r, t;

    for (i = 0; i < NPTS; i++) {
        id[i] = (unsigned char)i;
        gens[0][i] = (unsigned char)((i + 1) % NPTS);
        gens[1][i] = g2[i];
    }
    pinv(gens[0], ginv[0]);
    pinv(gens[1], ginv[1]);

    memset(htab, 0, sizeof(htab));
    Gn = 0;
    padd(id);
    for (q = 0; q < Gn; q++)
        for (k = 0; k < 2; k++) {
            pmul(Gel[q], gens[k], tmp);
            padd(tmp);
        }

    for (i = 0; i < Gn; i++) cls[i] = -1;
    r = 0;
    for (i = 0; i < Gn && r < MAXC; i++) {
        int sp = 0;
        if (cls[i] >= 0) continue;
        cls[i] = r;
        rep[r] = i;
        csz[r] = 1;
        stackv[sp++] = i;
        while (sp > 0) {
            int e = stackv[--sp];
            for (k = 0; k < 2; k++) {
                int x;
                pmul(ginv[k], Gel[e], tmp);
                pmul(tmp, gens[k], tmp2);
                x = pfind(tmp2);
                if (x >= 0 && cls[x] < 0) {
                    cls[x] = r;
                    csz[r]++;
                    stackv[sp++] = x;
                }
            }
        }
        r++;
    }
    nclasses = r;

    for (j = 0; j < r; j++) {
        pinv(Gel[rep[j]], tmp);
        invc[j] = cls[pfind(tmp)];
    }

    memset(cc, 0, sizeof(cc));
    for (l = 0; l < r; l++) {
        const unsigned char *z = Gel[rep[l]];
        for (q = 0; q < Gn; q++) {
            pinv(Gel[q], tmp);
            pmul(tmp, z, tmp2);
            cc[cls[q]][cls[pfind(tmp2)]][l]++;
        }
    }

    for (t = 0; t < 200; t++) {
        long long w[MAXC], v[MAXC], lam;
        int found = 0;
        bool bad = false;
        for (j = 0; j < r; j++) w[j] = mpow(t + 2, j + 1 + (long long)t * j);
        for (k = 0; k < r; k++)
            for (l = 0; l < r; l++) {
                long long s = 0;
                for (j = 0; j < r; j++) s = (s + w[j] * (cc[j][k][l] % PR)) % PR;
                M[k][l] = s;
            }
        for (lam = 0; lam < PR && !bad; lam++) {
            int nd;
            for (k = 0; k < r; k++)
                for (l = 0; l < r; l++) {
                    A[k][l] = M[k][l];
                    if (k == l) A[k][l] = (A[k][l] - lam + PR) % PR;
                }
            nd = nullvec(A, r, v);
            if (nd == 0) continue;
            if (nd > 1 || v[0] == 0 || found >= r) { bad = true; break; }
            {
                long long iv0 = minv(v[0]), S = 0, d2;
                int d, deg = 0;
                for (j = 0; j < r; j++) v[j] = v[j] * iv0 % PR;
                for (j = 0; j < r; j++)
                    S = (S + v[j] * v[invc[j]] % PR * minv(csz[j] % PR)) % PR;
                if (S == 0) { bad = true; break; }
                d2 = (long long)(Gn % PR) * minv(S) % PR;
                for (d = 1; (long long)d * d <= Gn; d++)
                    if (Gn % d == 0 && (long long)d * d % PR == d2) { deg = d; break; }
                if (deg == 0) { bad = true; break; }
                degs[found++] = deg;
            }
        }
        if (!bad && found == r) break;
    }

    for (i = 1; i < r; i++) {
        int x = degs[i];
        j = i - 1;
        while (j >= 0 && degs[j] > x) { degs[j + 1] = degs[j]; j--; }
        degs[j + 1] = x;
    }
    ready = true;
}

seq_t a(idx_t n) {
    if (!ready) compute();
    if (n < 1 || n > (idx_t)nclasses) return (seq_t)0;
    return (seq_t)degs[n - 1];
}

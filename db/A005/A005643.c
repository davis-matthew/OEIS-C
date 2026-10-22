/*
 * A005643  Number of labeled irreducible 2-connected graphs with n edges.
 * https://oeis.org/A005643
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of labeled 2-connected graphs on n nodes with all
 *            degrees >= 3 (homeomorphically irreducible). Computed by
 *            inclusion-exclusion over sets S of degree-2 nodes: suppressing S
 *            gives a 2-connected multigraph H in which each node pair carries
 *            at most one real edge plus any set of signed c...
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static int BIG;
static size_t WD;
static seq_t *BN;
static seq_t *cache = 0;
static idx_t cacheN = 0;

#define IX(i, k) ((size_t)(i) * WD + (size_t)(k))

static void nrm(seq_t *p, seq_t *m)
{
    if (BIG) {
        if (*p >= *m) { *p -= *m; *m = 0; }
        else { *m -= *p; *p = 0; }
    }
}

/* out += (neg ? -1 : 1) * zb * (A x-binomial-conv B), for k <= kmax */
static void cma(seq_t *op, seq_t *om, const seq_t *ap, const seq_t *am,
                const seq_t *bp, const seq_t *bm, seq_t zb, int neg, long kmax)
{
    long k, j;
    for (k = 0; k <= kmax; k++) {
        seq_t sp = 0, sm = 0;
        for (j = 0; j <= k; j++) {
            seq_t t1, t2;
            if (ap[j] == 0 && am[j] == 0) continue;
            t1 = ap[j] * bp[k - j] + am[j] * bm[k - j];
            t2 = ap[j] * bm[k - j] + am[j] * bp[k - j];
            sp += BN[IX(k, j)] * t1;
            sm += BN[IX(k, j)] * t2;
        }
        sp *= zb; sm *= zb;
        if (neg) { op[k] += sm; om[k] += sp; }
        else { op[k] += sp; om[k] += sm; }
        nrm(&op[k], &om[k]);
    }
}

static void build(idx_t N)
{
    size_t SZ, i, j, h, k, m, s, n;
    seq_t *cp, *cm, *Fp, *Fm, *emp, *emm, *Cp, *Cm, *Dp, *Dm, *Rp, *Rm;
    seq_t *Pap, *Pam, *Pbp, *Pbm, *Ap, *Am, *Bp, *Bm, *tp, f, p2;

    BIG = seq_is_big();
    WD = (size_t)N + 1;
    SZ = WD * WD;

    BN = alloc_zero(SZ);
    for (i = 0; i <= N; i++) {
        BN[IX(i, 0)] = 1;
        for (j = 1; j <= i; j++) BN[IX(i, j)] = BN[IX(i - 1, j - 1)] + BN[IX(i - 1, j)];
    }

    /* chain counts c_k = (-1)^k k! */
    cp = alloc_zero(WD); cm = alloc_zero(WD);
    f = 1;
    for (k = 1; k <= N; k++) {
        f *= (seq_t)k;
        if (k & 1) cm[k] = f; else cp[k] = f;
    }

    /* F[h] = W^C(h,2) = 2^e exp(e c) */
    Fp = alloc_zero(SZ); Fm = alloc_zero(SZ);
    emp = alloc_zero(WD); emm = alloc_zero(WD);
    p2 = 1;
    for (h = 0; h <= N; h++) {
        size_t kmax = N - h;
        idx_t e = (idx_t)h * (h ? h - 1 : 0) / 2;
        seq_t ev = (seq_t)e;
        if (h >= 2) for (i = 0; i + 1 < h; i++) p2 *= 2;
        emp[0] = 1; emm[0] = 0;
        for (k = 1; k <= kmax; k++) {
            seq_t sp = 0, sm = 0;
            for (i = 1; i <= k; i++) {
                seq_t b = BN[IX(k - 1, i - 1)];
                sp += b * (cp[i] * emp[k - i] + cm[i] * emm[k - i]);
                sm += b * (cp[i] * emm[k - i] + cm[i] * emp[k - i]);
            }
            emp[k] = ev * sp; emm[k] = ev * sm;
            nrm(&emp[k], &emm[k]);
        }
        for (k = 0; k <= kmax; k++) {
            Fp[IX(h, k)] = p2 * emp[k];
            Fm[IX(h, k)] = p2 * emm[k];
        }
    }

    /* connected: C = log F */
    Cp = alloc_zero(SZ); Cm = alloc_zero(SZ);
    for (h = 1; h <= N; h++) {
        long kmax = (long)(N - h);
        for (k = 0; k <= (size_t)kmax; k++) {
            Cp[IX(h, k)] = Fp[IX(h, k)];
            Cm[IX(h, k)] = Fm[IX(h, k)];
        }
        for (i = 1; i < h; i++)
            cma(&Cp[IX(h, 0)], &Cm[IX(h, 0)], &Cp[IX(i, 0)], &Cm[IX(i, 0)],
                &Fp[IX(h - i, 0)], &Fm[IX(h - i, 0)], BN[IX(h - 1, i - 1)], 1, kmax);
    }

    /* D = log C'(z), C'[m] = C[m+1] */
    Dp = alloc_zero(SZ); Dm = alloc_zero(SZ);
    for (m = 1; m + 1 <= N; m++) {
        long kmax = (long)(N - 1 - m);
        for (k = 0; k <= (size_t)kmax; k++) {
            Dp[IX(m, k)] = Cp[IX(m + 1, k)];
            Dm[IX(m, k)] = Cm[IX(m + 1, k)];
        }
        for (i = 1; i < m; i++)
            cma(&Dp[IX(m, 0)], &Dm[IX(m, 0)], &Dp[IX(i, 0)], &Dm[IX(i, 0)],
                &Cp[IX(m - i + 1, 0)], &Cm[IX(m - i + 1, 0)], BN[IX(m - 1, i - 1)], 1, kmax);
    }

    /* R = z C'(z): R[s] = s * C[s] */
    Rp = alloc_zero(SZ); Rm = alloc_zero(SZ);
    for (s = 1; s + 1 <= N; s++)
        for (k = 0; k + 1 + s <= N; k++) {
            Rp[IX(s, k)] = (seq_t)s * Cp[IX(s, k)];
            Rm[IX(s, k)] = (seq_t)s * Cm[IX(s, k)];
        }

    /* solve D = B'(R) */
    Pap = alloc_zero(SZ); Pam = alloc_zero(SZ);
    Pbp = alloc_zero(SZ); Pbm = alloc_zero(SZ);
    Ap = alloc_zero(SZ); Am = alloc_zero(SZ);
    Bp = alloc_zero(SZ); Bm = alloc_zero(SZ);
    Pap[IX(0, 0)] = 1;
    for (j = 1; j + 1 <= N; j++) {
        for (m = j; m + 1 <= N; m++) {
            long kmax = (long)(N - 1 - m);
            for (k = 0; k <= (size_t)kmax; k++) { Pbp[IX(m, k)] = 0; Pbm[IX(m, k)] = 0; }
            for (s = 1; s <= m - j + 1; s++)
                cma(&Pbp[IX(m, 0)], &Pbm[IX(m, 0)], &Rp[IX(s, 0)], &Rm[IX(s, 0)],
                    &Pap[IX(m - s, 0)], &Pam[IX(m - s, 0)], BN[IX(m - 1, s - 1)], 0, kmax);
        }
        for (k = 0; k + 1 + j <= N; k++) {
            Bp[IX(j, k)] = Dp[IX(j, k)] + Am[IX(j, k)];
            Bm[IX(j, k)] = Dm[IX(j, k)] + Ap[IX(j, k)];
            nrm(&Bp[IX(j, k)], &Bm[IX(j, k)]);
        }
        for (m = j + 1; m + 1 <= N; m++)
            cma(&Ap[IX(m, 0)], &Am[IX(m, 0)], &Bp[IX(j, 0)], &Bm[IX(j, 0)],
                &Pbp[IX(m, 0)], &Pbm[IX(m, 0)], (seq_t)1, 0, (long)(N - 1 - m));
        tp = Pap; Pap = Pbp; Pbp = tp;
        tp = Pam; Pam = Pbm; Pbm = tp;
    }

    if (cache) release(cache, cacheN + 1);
    cache = alloc_zero(WD);
    for (n = 3; n <= N; n++) {
        seq_t sp = 0, sm = 0, T;
        size_t k2 = n - 2;
        for (h = 2; h <= n; h++) {
            sp += BN[IX(n, h)] * Bp[IX(h - 1, n - h)];
            sm += BN[IX(n, h)] * Bm[IX(h - 1, n - h)];
        }
        if (k2 == 0) sm += BN[IX(n, 2)];
        else { sp += BN[IX(n, 2)] * cm[k2]; sm += BN[IX(n, 2)] * cp[k2]; }
        T = (seq_t)(n - 1);
        for (i = 3; i < n; i++) T *= (seq_t)i;
        if (n % 2 == 0) sm += T; else sp += T;
        cache[n] = sp - sm;
    }
    cacheN = N;

    release(cp, WD); release(cm, WD); release(emp, WD); release(emm, WD);
    release(Fp, SZ); release(Fm, SZ); release(Cp, SZ); release(Cm, SZ);
    release(Dp, SZ); release(Dm, SZ); release(Rp, SZ); release(Rm, SZ);
    release(Pap, SZ); release(Pam, SZ); release(Pbp, SZ); release(Pbm, SZ);
    release(Ap, SZ); release(Am, SZ); release(Bp, SZ); release(Bm, SZ);
    release(BN, SZ);
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    if (n > cacheN) {
        idx_t N = n;
        if (cacheN && cacheN + 4 > N) N = cacheN + 4;
        build(N);
    }
    return cache[n];
}

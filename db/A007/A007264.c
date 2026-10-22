/*
 * A007264  McKay-Thompson series of class 7A for Monster.
 * https://oeis.org/A007264
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] (h+7)^2/h - 10*[n=0] = h_n + 49*(1/h)_n for n != 0,
 *            a(0) = 0, where h = (eta(q)/eta(q^7))^4 = q^-1 * E(q)^4 /
 *            E(q^7)^4, E(q) = prod (1-q^k); computed with sparse pentagonal /
 *            Jacobi (E^3) recurrences in sign-magnitude form
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 28, u64 n <= 105, u128 n <= 392,
 *            big any n
 * Helpers:   addterm, alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/addterm.h"

static seq_t *Pm = 0, *Rm = 0;
static signed char *Ps = 0, *Rs = 0;
static idx_t cap = 0;

static void fin(seq_t *pos, seq_t *neg, seq_t *mag, signed char *sg, idx_t i)
{
    if (*pos >= *neg) { mag[i] = *pos - *neg; sg[i] = 1; }
    else { mag[i] = *neg - *pos; sg[i] = -1; }
}

/* out_n = sum_k cf_k * in_{n - scale*off_k} */
static void smul(const seq_t *im, const signed char *is, seq_t *om, signed char *os,
                 idx_t L, const long long *off, const long long *cf, int cnt, long long scale)
{
    idx_t n;
    int k;
    for (n = 0; n < L; n++) {
        seq_t pos = 0, neg = 0;
        for (k = 0; k < cnt; k++) {
            long long d = off[k] * scale;
            if (d > n) break;
            addterm(&pos, &neg, cf[k], &im[n - d], is[n - d]);
        }
        fin(&pos, &neg, om, os, n);
    }
}

/* out = in / S, S_0 = 1: out_n = in_n - sum_{off>0} cf * out_{n-off} */
static void sdiv(const seq_t *im, const signed char *is, seq_t *om, signed char *os,
                 idx_t L, const long long *off, const long long *cf, int cnt)
{
    idx_t n;
    int k;
    for (n = 0; n < L; n++) {
        seq_t pos = 0, neg = 0;
        if (is[n] > 0) pos += im[n]; else neg += im[n];
        for (k = 0; k < cnt; k++) {
            if (off[k] == 0) continue;
            if (off[k] > n) break;
            addterm(&pos, &neg, -cf[k], &om[n - off[k]], os[n - off[k]]);
        }
        fin(&pos, &neg, om, os, n);
    }
}

static void build(idx_t L)
{
    long long *po, *pc, *to, *tc;
    int pn = 0, tn = 0;
    long long k;
    idx_t i;
    seq_t *A, *B, *C;
    signed char *As, *Bs, *Cs;

    po = (long long *)malloc(sizeof(long long) * 4 * 1000);
    pc = (long long *)malloc(sizeof(long long) * 4 * 1000);
    to = (long long *)malloc(sizeof(long long) * 2000);
    tc = (long long *)malloc(sizeof(long long) * 2000);
    po[pn] = 0; pc[pn] = 1; pn++;
    for (k = 1; k * (3 * k - 1) / 2 < L; k++) {
        long long s = (k & 1) ? -1 : 1;
        po[pn] = k * (3 * k - 1) / 2; pc[pn] = s; pn++;
        if (k * (3 * k + 1) / 2 < L) { po[pn] = k * (3 * k + 1) / 2; pc[pn] = s; pn++; }
    }
    for (k = 0; k * (k + 1) / 2 < L; k++) {
        to[tn] = k * (k + 1) / 2;
        tc[tn] = (k & 1) ? -(2 * k + 1) : (2 * k + 1);
        tn++;
    }

    A = alloc_zero(L); B = alloc_zero(L); C = alloc_zero(L);
    As = (signed char *)malloc(L); Bs = (signed char *)malloc(L); Cs = (signed char *)malloc(L);
    for (i = 0; i < L; i++) { As[i] = 1; Bs[i] = 1; Cs[i] = 1; }

    /* A = delta; B = 1/E^3; C = F = 1/E^4 */
    A[0] = 1;
    sdiv(A, As, B, Bs, L, to, tc, tn);
    sdiv(B, Bs, C, Cs, L, po, pc, pn);

    if (Rm) { release(Rm, cap); release(Pm, cap); free(Rs); free(Ps); }
    Rm = alloc_zero(L); Pm = alloc_zero(L);
    Rs = (signed char *)malloc(L); Ps = (signed char *)malloc(L);

    /* R = F * E(q^7) * E(q^7)^3 */
    smul(C, Cs, A, As, L, po, pc, pn, 7);
    smul(A, As, Rm, Rs, L, to, tc, tn, 7);

    /* G7(q) = F(q^7); P = G7 * E^3 * E */
    for (i = 0; i < L; i++) {
        if (i % 7 == 0) { A[i] = C[i / 7]; As[i] = Cs[i / 7]; }
        else { A[i] = 0; As[i] = 1; }
    }
    smul(A, As, B, Bs, L, to, tc, tn, 1);
    smul(B, Bs, Pm, Ps, L, po, pc, pn, 1);

    release(A, L); release(B, L); release(C, L);
    free(As); free(Bs); free(Cs);
    free(po); free(pc); free(to); free(tc);
    cap = L;
}

seq_t a(idx_t n)
{
    idx_t L = n + 2;
    seq_t pos = 0, neg = 0;
    if (n == 0) return 0;
    if (L > cap) {
        idx_t nl = cap * 2;
        if (nl < L) nl = L;
        if (nl < 64) nl = 64;
        build(nl);
    }
    if (Ps[n + 1] > 0) pos += Pm[n + 1]; else neg += Pm[n + 1];
    if (n >= 1) {
        if (Rs[n - 1] > 0) pos += 49 * Rm[n - 1]; else neg += 49 * Rm[n - 1];
    }
    return pos - neg;
}

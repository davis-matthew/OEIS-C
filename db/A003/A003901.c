/*
 * A003901  Degrees of irreducible representations of group L5(2).
 * https://oeis.org/A003901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Green's degree formula for GL(n,q), here n=5, q=2. Each
 *            irreducible character of GL(5,2) assigns a partition lambda_f to
 *            every monic irreducible polynomial f != x over GF(2), of degree
 *            d_f, with Sum d_f*|lambda_f| = 5. Its degree is
 *            prod_{i=1..5}(q^i-1) * prod_f q^(d_f*n(lambda_f)) /
 *            prod_{hooks...
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   cmp_u64, ipow_u, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/cmp_u64.h"
#include "../utils/ipow_u.h"
#include "../utils/mobius.h"

#define GN 5
#define GQ 2

static int np = 0;
static int ptab[32][GN + 1];
static int plen[32], psz[32];
static int slots[64], nslots = 0;
static uint64_t degs[256];
static int cnt = -1;

static void gen_parts(int k, int rem, int maxp, int *cur, int len) {
    int i, p;
    if (rem == 0) {
        for (i = 0; i < len; i++) ptab[np][i] = cur[i];
        plen[np] = len;
        psz[np] = k;
        np++;
        return;
    }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        cur[len] = p;
        gen_parts(k, rem - p, p, cur, len + 1);
    }
}

static void part_factors(int pi, int d, uint64_t *num, uint64_t *den) {
    int i, j, L = plen[pi];
    int conj[GN + 1];
    int nl = 0;
    uint64_t dn = 1;
    for (j = 0; j < GN + 1; j++) conj[j] = 0;
    for (i = 0; i < L; i++) {
        nl += i * ptab[pi][i];
        for (j = 0; j < ptab[pi][i]; j++) conj[j]++;
    }
    for (i = 0; i < L; i++)
        for (j = 0; j < ptab[pi][i]; j++) {
            int h = ptab[pi][i] + conj[j] - i - j - 1;
            dn *= ipow_u(GQ, d * h) - 1;
        }
    *num = ipow_u(GQ, d * nl);
    *den = dn;
}

static void gen(int s, int rem, uint64_t num, uint64_t den) {
    int pi;
    if (rem == 0) {
        degs[cnt++] = num / den;
        return;
    }
    if (s == nslots) return;
    gen(s + 1, rem, num, den);
    for (pi = 0; pi < np; pi++) {
        int d = slots[s];
        if (psz[pi] >= 1 && d * psz[pi] <= rem) {
            uint64_t a, b;
            part_factors(pi, d, &a, &b);
            gen(s + 1, rem - d * psz[pi], num * a, den * b);
        }
    }
}

static void build(void) {
    int k, d, e, i;
    int cur[GN + 1];
    uint64_t psi = 1;
    for (k = 1; k <= GN; k++) gen_parts(k, k, k, cur, 0);
    for (d = 1; d <= GN; d++) {
        int64_t s = 0;
        int64_t nd;
        for (e = 1; e <= d; e++)
            if (d % e == 0) s += (int64_t)mobius(d / e) * (int64_t)ipow_u(GQ, e);
        nd = s / d;
        if (d == 1) nd -= 1; /* exclude f = x */
        for (i = 0; i < nd; i++) slots[nslots++] = d;
    }
    for (i = 1; i <= GN; i++) psi *= ipow_u(GQ, i) - 1;
    cnt = 0;
    gen(0, GN, psi, 1);
    qsort(degs, (size_t)cnt, sizeof(uint64_t), cmp_u64);
}

seq_t a(idx_t n) {
    if (cnt < 0) build();
    if (n < 1 || n > (idx_t)cnt) return (seq_t)0;
    return (seq_t)degs[n - 1];
}

/*
 * A000623  Number of monosubstituted alkanes C(n)H(2n+1)-X of the form shown
 *          in the Comments lines that are stereoisomers.
 * https://oeis.org/A000623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with s(x) = 1 + x*(s(x)^3 + 2*s(x^3))/3 (stereo alkyl radicals,
 *            A000625 with empty term) and t = s - 1, the stereoisomer count
 *            of X-C(R)(R')(R'') is st(m) = [x^m] (t(x)^3 + 2*t(x^3))/3, m =
 *            n-1. Structures that have only one stereoisomer are built from
 *            "rigid" alkyls r(x) = 1 + x*r(x)*r(x^2), so...
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 25, u64 n <= 44, u128 n <= 82,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"

static int a623_fixed = -1;
static seq_t a623_inv3;
static seq_t *a623_S = 0, *a623_S2 = 0, *a623_S3 = 0, *a623_R = 0;
static long long a623_cap = -1;
static long long a623_done = -1;

static void a623_init(void)
{
    seq_t m1 = 0;
    int it;
    m1 -= 1;
    a623_fixed = (m1 > 0) ? 1 : 0;
    a623_inv3 = 3;
    if (a623_fixed) {
        for (it = 0; it < 8; it++) {
            seq_t t = 2;
            t -= a623_inv3 * 3;
            a623_inv3 = a623_inv3 * t;
        }
    }
}

static seq_t a623_div3(seq_t x)
{
    if (a623_fixed) {
        seq_t r = x * a623_inv3;
        return r;
    }
    return x / 3;
}

static void a623_extend(long long N)
{
    long long i, m, j;
    seq_t *nS, *nS2, *nS3, *nR;
    if (N <= a623_cap) return;
    nS = alloc_zero((idx_t)(N + 1));
    nS2 = alloc_zero((idx_t)(N + 1));
    nS3 = alloc_zero((idx_t)(N + 1));
    nR = alloc_zero((idx_t)(N + 1));
    for (i = 0; i <= a623_cap; i++) {
        nS[i] = a623_S[i];
        nS2[i] = a623_S2[i];
        nS3[i] = a623_S3[i];
        nR[i] = a623_R[i];
    }
    a623_S = nS; a623_S2 = nS2; a623_S3 = nS3; a623_R = nR;
    a623_cap = N;
    for (m = a623_done + 1; m <= N; m++) {
        seq_t t;
        if (m == 0) {
            a623_S[0] = 1;
        } else {
            t = a623_S3[m - 1];
            if ((m - 1) % 3 == 0) t += a623_S[(m - 1) / 3] * 2;
            a623_S[m] = a623_div3(t);
        }
        t = 0;
        for (i = 0; i < m - i; i++) t += a623_S[i] * a623_S[m - i];
        t += t;
        if (m % 2 == 0) t += a623_S[m / 2] * a623_S[m / 2];
        a623_S2[m] = t;
        t = 0;
        for (i = 0; i <= m; i++) t += a623_S[i] * a623_S2[m - i];
        a623_S3[m] = t;
        if (m == 0) {
            a623_R[0] = 1;
        } else {
            t = 0;
            for (j = 0; 2 * j <= m - 1; j++) t += a623_R[m - 1 - 2 * j] * a623_R[j];
            a623_R[m] = t;
        }
    }
    a623_done = N;
}

seq_t a(idx_t n)
{
    long long m, j;
    seq_t ts, u;
    if (a623_fixed < 0) a623_init();
    if (n < 2) return 0;
    m = (long long)(n - 1);
    if (m > a623_cap) {
        long long nc = 2 * a623_cap;
        if (nc < m) nc = m;
        if (nc < 16) nc = 16;
        a623_extend(nc);
    }
    ts = a623_S3[m];
    ts += a623_S[m] * 3;
    if (m % 3 == 0) ts += a623_S[m / 3] * 2;
    ts -= a623_S2[m] * 3;
    ts = a623_div3(ts);
    u = 0;
    for (j = 1; 2 * j + 1 <= m; j++) u += a623_R[m - 2 * j] * a623_R[j];
    return ts - u;
}

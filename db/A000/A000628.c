/*
 * A000628  Number of n-node unrooted steric quartic trees; number of n-carbon
 *          alkanes C(n)H(2n+2) taking stereoisomers into account.
 * https://oeis.org/A000628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Otter-type dissimilarity count for steric trees. Planted steric
 *            trees: R(x) = 1 + x*(R(x)^3 + 2*R(x^3))/3, R(0) = 1. a(n) =
 *            [x^(n-1)] Z(A4; R) - (number of unordered pairs {i,j} of
 *            distinct planted trees with sizes summing to n, both nonempty),
 *            where Z(A4) = (s1^4 + 3*s2^2 + 8*s1*s3)/12 is taken...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 26, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   alloc_zero, tri_down, tri_up
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/tri_down.h"
#include "../utils/tri_up.h"

static seq_t *R_ = 0, *S2_ = 0, *S3_ = 0, *M_ = 0, *P_ = 0;
static long long cap_ = 0;
static int fixed_ = -1;
static seq_t inv3_;

static void init_mode(void)
{
    seq_t t = 1;
    int i;
    fixed_ = 0;
    for (i = 0; i < 200; i++) {
        t = t * 2;
        if (t == 0) { fixed_ = 1; break; }
    }
    if (fixed_) {
        seq_t x = 3;
        for (i = 0; i < 8; i++) {
            seq_t u = (seq_t)3 * x;
            u = (seq_t)2 - u;
            x = x * u;
        }
        inv3_ = x;
    }
}

static seq_t div3(seq_t v)
{
    if (fixed_) return v * inv3_;
    return v / 3;
}

static void build(long long N)
{
    long long m, i;
    R_ = alloc_zero((idx_t)(N + 1));
    S2_ = alloc_zero((idx_t)(N + 1));
    S3_ = alloc_zero((idx_t)(N + 1));
    M_ = alloc_zero((idx_t)(N + 1));
    P_ = alloc_zero((idx_t)(N + 1));
    R_[0] = 1;
    S2_[0] = 1;
    S3_[0] = 1;
    M_[0] = 1;
    P_[0] = 0;
    for (m = 1; m <= N; m++) {
        seq_t v = S3_[m - 1];
        seq_t s, s3, pm;
        if ((m - 1) % 3 == 0) v += 2 * R_[(m - 1) / 3];
        R_[m] = div3(v);
        s = 0;
        for (i = 0; i <= m; i++) s += R_[i] * R_[m - i];
        S2_[m] = s;
        s3 = 0;
        for (i = 0; i <= m; i++) s3 += R_[i] * S2_[m - i];
        S3_[m] = s3;
        pm = 0;
        for (i = 0; 2 * i < m; i++) pm += R_[i] * R_[m - i];
        if (m % 2 == 0) {
            M_[m] = pm + tri_up(R_[m / 2]);
            P_[m] = pm + tri_down(R_[m / 2]);
        } else {
            M_[m] = pm;
            P_[m] = pm;
        }
    }
    cap_ = N;
}

seq_t a(idx_t n)
{
    long long nn, m, i, j;
    seq_t W, T, V, E;
    if (n == 0) return 1;
    if (fixed_ < 0) init_mode();
    nn = (long long)n;
    if (nn > cap_) {
        long long N = 2 * cap_;
        if (N < nn) N = nn;
        if (N < 32) N = 32;
        build(N);
    }
    m = nn - 1;
    W = 0;
    for (i = 0; i <= m; i++) W += P_[i] * M_[m - i];
    if (m % 2 == 0) W += S2_[m / 2];
    T = 0;
    for (j = 0; 3 * j <= m; j++) T += R_[j] * R_[m - 3 * j];
    V = div3(W + 2 * T);
    E = 0;
    for (i = 1; 2 * i < nn; i++) E += R_[i] * R_[nn - i];
    if (nn % 2 == 0) E += tri_down(R_[nn / 2]);
    return V - E;
}

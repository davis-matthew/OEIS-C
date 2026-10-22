/*
 * A004033  Theta series of lattice A_2 tensor E_8 (dimension 16, det. 6561,
 *          min. norm 4). Also theta series of Eisenstein version of E_8
 *          lattice.
 * https://oeis.org/A004033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = (240*sigma_7(n) +
 *            19440*sigma_7(n/3) - 240*s(n))/41, where sigma_7(n/3) = 0 if 3
 *            does not divide n, and s(n) = [x^n] x*f(-x)^6*f(-x^3)^6*a(x)^2
 *            (the weight-8 level-3 newform: q + 6q^2 - 27q^3 - 92q^4 + ...).
 *            Here f(-x)^6 f(-x^3)^6 = J(x)^2 J(x^3)^2 with J(x) = Sum_{k>...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 437, u128 n <= 1000,
 *            big any n
 * Helpers:   from_i128, pow7, sigma7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/from_i128.h"
#include "../utils/pow7.h"
#include "../utils/sigma7.h"

static int64_t *Cc = NULL;
static int64_t *A2c = NULL;
static idx_t cap = 0;

static void build(idx_t N)
{
    idx_t i, j, m, d;
    int64_t *J2 = (int64_t *)calloc(N, sizeof(int64_t));
    int64_t *sg = (int64_t *)calloc(N, sizeof(int64_t));
    free(Cc);
    free(A2c);
    Cc = (int64_t *)calloc(N, sizeof(int64_t));
    A2c = (int64_t *)calloc(N, sizeof(int64_t));
    for (i = 0; i * (i + 1) / 2 < N; i++) {
        idx_t ti = i * (i + 1) / 2;
        int64_t si = (i & 1) ? -(int64_t)(2 * i + 1) : (int64_t)(2 * i + 1);
        for (j = 0; ti + j * (j + 1) / 2 < N; j++) {
            int64_t sj = (j & 1) ? -(int64_t)(2 * j + 1) : (int64_t)(2 * j + 1);
            J2[ti + j * (j + 1) / 2] += si * sj;
        }
    }
    for (m = 0; m < N; m++) {
        int64_t acc = 0;
        for (j = 0; 3 * j <= m; j++)
            acc += J2[j] * J2[m - 3 * j];
        Cc[m] = acc;
    }
    for (d = 1; d < N; d++)
        for (m = d; m < N; m += d)
            sg[m] += (int64_t)d;
    A2c[0] = 1;
    for (m = 1; m < N; m++)
        A2c[m] = 12 * (sg[m] - ((m % 3 == 0) ? 3 * sg[m / 3] : 0));
    free(J2);
    free(sg);
    cap = N;
}

seq_t a(idx_t n)
{
    idx_t i;
    __int128 s = 0, t, q3;
    long long r1, r2, r3, R;
    seq_t S1, S3, q1, q2, res;
    if (n == 0)
        return 1;
    if (n > cap) {
        idx_t nc = cap * 2;
        if (nc < n)
            nc = n;
        build(nc);
    }
    for (i = 0; i < n; i++)
        s += (__int128)Cc[i] * (__int128)A2c[n - 1 - i];
    S1 = sigma7(n);
    S3 = (n % 3 == 0) ? sigma7(n / 3) : (seq_t)0;
    q1 = S1 / 41;
    r1 = (long long)(S1 % 41);
    q2 = S3 / 41;
    r2 = (long long)(S3 % 41);
    t = s * 240;
    r3 = (long long)(t % 41);
    if (r3 < 0)
        r3 += 41;
    q3 = (t - r3) / 41;
    R = (240 * r1 + 19440 * r2 - r3) / 41;
    res = q1 * 240 + q2 * 19440;
    if (R > 0)
        res += (idx_t)R;
    if (q3 < 0)
        res += from_i128(-q3);
    if (q3 > 0)
        res -= from_i128(q3);
    if (R < 0)
        res -= (idx_t)(-R);
    return res;
}

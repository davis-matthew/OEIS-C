/*
 * A006909  Theta series of laminated lattice LAMBDA_10.
 * https://oeis.org/A006909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Lambda_10 = union over i=0..3 of (g_i + sqrt2*E8) (+) (r_i +
 *            2*A2), with g_i the classes of a totally singular 2-space in
 *            E8/2E8 and r_i the classes of A2/2A2. So a(n) = Sum_{j+k=n}
 *            A(j)*B(k) + 3*Sum_{j+k=n} C(j)*D(k), where A(j) = [j even] *
 *            (j==0 ? 1 : 240*sigma_3(j/2)), B(k) = [4|k] * r_A2(k/4...
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 123, u64 n <= 9969,
 *            u128 n <= 9969, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *s3 = NULL;   /* sigma_3(m), m <= 2*cap */
static int64_t *rc = NULL;    /* Sum_{d|m} chi_{-3}(d), m <= cap */
static idx_t cap = 0;

static void build(idx_t N)
{
    idx_t M = 2 * N, d, m;
    free(s3);
    free(rc);
    s3 = (uint64_t *)calloc((size_t)(M + 1), sizeof(uint64_t));
    rc = (int64_t *)calloc((size_t)(N + 1), sizeof(int64_t));
    for (d = 1; d <= M; d++) {
        uint64_t c = (uint64_t)d * d * d;
        for (m = d; m <= M; m += d) s3[m] += c;
    }
    for (d = 1; d <= N; d++) {
        int64_t ch = (d % 3 == 1) ? 1 : (d % 3 == 2) ? -1 : 0;
        if (ch == 0) continue;
        for (m = d; m <= N; m += d) rc[m] += ch;
    }
    cap = N;
}

static uint64_t fA(idx_t j) { if (j & 1) return 0; return j == 0 ? 1 : 240 * s3[j / 2]; }
static uint64_t fB(idx_t k) { if (k % 4) return 0; return k == 0 ? 1 : (uint64_t)(6 * rc[k / 4]); }
static uint64_t fC(idx_t j)
{
    if (j == 0) return 0;
    return 16 * (s3[2 * j] - ((j & 1) ? 0 : s3[j / 2])) / 9;
}
static uint64_t fD(idx_t k) { if (!(k & 1)) return 0; return (uint64_t)(2 * rc[k]); }

seq_t a(idx_t n)
{
    seq_t s1 = 0, s2 = 0;
    idx_t k;
    if (n > cap || s3 == NULL) {
        idx_t N = cap * 2;
        if (N < n) N = n;
        if (N < 16) N = 16;
        build(N);
    }
    for (k = 0; k <= n; k += 4) {
        if ((n - k) & 1) continue;
        s1 += (seq_t)fA(n - k) * (seq_t)fB(k);
    }
    for (k = 1; k <= n; k += 2)
        s2 += (seq_t)fC(n - k) * (seq_t)fD(k);
    return s1 + 3 * s2;
}

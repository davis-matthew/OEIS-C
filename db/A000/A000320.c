/*
 * A000320  Generalized tangent numbers d(5,n).
 * https://oeis.org/A000320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(2n-1,2k) * 25^k * E(2k) *
 *            (-1)^(n-1-k) * (1 + 3^(2(n-k)-1)), where E(2k) are the secant
 *            (Euler) numbers A000364 (from sec(5x) * (sin(x) + sin(3x)))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A320_MAXN 10010
#define A320_MAXR (2 * A320_MAXN + 8)

static seq_t a320_bufA[A320_MAXR], a320_bufB[A320_MAXR];
static seq_t *a320_cur = a320_bufA, *a320_nxt = a320_bufB;
static seq_t a320_zz[A320_MAXR];
static long long a320_zk = -1;

static seq_t a320_P[A320_MAXR];
static seq_t a320_T3[A320_MAXN + 2];

/* zigzag numbers via Seidel boustrophedon, up to index K */
static void a320_zig_upto(long long K)
{
    if (a320_zk < 0) {
        a320_cur[0] = 1;
        a320_zz[0] = 1;
        a320_zk = 0;
    }
    while (a320_zk < K) {
        long long k = a320_zk + 1, i;
        seq_t *t;
        a320_nxt[0] = 0;
        for (i = 1; i <= k; i++)
            a320_nxt[i] = a320_nxt[i - 1] + a320_cur[k - i];
        a320_zz[k] = a320_nxt[k];
        t = a320_cur; a320_cur = a320_nxt; a320_nxt = t;
        a320_zk = k;
    }
}

seq_t a(idx_t n)
{
    long long N = 2 * (long long)n - 1, r, j, k, m;
    long long nn = (long long)n;
    seq_t pos = 0, neg = 0, p25 = 1;

    a320_zig_upto(2 * nn);

    /* Pascal row 2n-1 */
    a320_P[0] = 1;
    for (r = 1; r <= N; r++) {
        a320_P[r] = 1;
        for (j = r - 1; j >= 1; j--)
            a320_P[j] += a320_P[j - 1];
    }

    /* T3[m] = 3^(2m-1) */
    a320_T3[1] = 3;
    for (m = 2; m <= nn; m++)
        a320_T3[m] = a320_T3[m - 1] * 9;

    for (k = 0; k < nn; k++) {
        seq_t term;
        m = nn - k;
        term = a320_P[2 * k] * p25 * a320_zz[2 * k] * (a320_T3[m] + 1);
        if (((m - 1) & 1) == 0)
            pos += term;
        else
            neg += term;
        p25 *= 25;
    }
    return pos - neg;
}

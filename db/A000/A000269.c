/*
 * A000269  Number of trees with n nodes, 3 of which are labeled.
 * https://oeis.org/A000269
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: A(x) = B(x)^3*(3-2*B(x))/(1-B(x))^3, B(x) = g.f. of
 *            A000081. Rewritten with nonnegative coefficients: with F =
 *            B/(1-B), A = 3*B*F^2 + B*F^3 = F^2*(2*B + F). B = sum r(m) x^m,
 *            r(1) = 1, r(m+1) = (1/m)*Sum_{k=1..m} s(k)*r(m+1-k), s(k) =
 *            Sum_{d|k} d*r(d); F(m) = r(m) + Sum_{j=1..m-1} r(j)*F(m-...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A269_MAXN 32768

static seq_t a269_r[A269_MAXN + 1];
static seq_t a269_s[A269_MAXN + 1];
static seq_t a269_F[A269_MAXN + 1];
static seq_t a269_G[A269_MAXN + 1];
static idx_t a269_have = 0; /* arrays valid for indices 1..a269_have */

static void a269_extend(idx_t N)
{
    idx_t m, j, k;
    if (a269_have == 0) {
        a269_r[0] = 0; a269_s[0] = 0; a269_F[0] = 0; a269_G[0] = 0;
        a269_r[1] = 1;
        a269_s[1] = 1;
        a269_F[1] = 1;
        a269_G[1] = 0;
        a269_have = 1;
    }
    for (m = a269_have + 1; m <= N; m++) {
        /* r(m) = (1/(m-1)) * Sum_{k=1..m-1} s(k) * r(m-k) */
        seq_t acc = 0;
        for (k = 1; k < m; k++)
            acc += a269_s[k] * a269_r[m - k];
        a269_r[m] = acc / (seq_t)(m - 1);

        /* s(m) = Sum_{d|m} d*r(d) */
        {
            seq_t sm = 0;
            idx_t d;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    sm += (seq_t)d * a269_r[d];
                    if (e != d) sm += (seq_t)e * a269_r[e];
                }
            }
            a269_s[m] = sm;
        }

        /* F(m) = r(m) + Sum_{j=1..m-1} r(j) * F(m-j) */
        {
            seq_t f = a269_r[m];
            for (j = 1; j < m; j++)
                f += a269_r[j] * a269_F[m - j];
            a269_F[m] = f;
        }

        /* G(m) = Sum_{j=1..m-1} F(j) F(m-j) */
        {
            seq_t g = 0;
            for (j = 1; 2 * j < m; j++)
                g += a269_F[j] * a269_F[m - j];
            g *= 2;
            if (m % 2 == 0)
                g += a269_F[m / 2] * a269_F[m / 2];
            a269_G[m] = g;
        }
        a269_have = m;
    }
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t j;
    if (n < 3 || n > A269_MAXN) return 0;
    if (a269_have < n) a269_extend(n);
    for (j = 2; j + 1 <= n; j++) {
        seq_t t = a269_r[n - j];
        t *= 2;
        t += a269_F[n - j];
        res += a269_G[j] * t;
    }
    return res;
}

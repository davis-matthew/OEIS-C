/*
 * A000107  Number of rooted trees with n nodes and a single labeled node;
 *          pointed rooted trees; vertebrates.
 * https://oeis.org/A000107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. A(x) = R(x)/(1-R(x)), R(x) = g.f. of A000081, so a(n) =
 *            r(n) + Sum_{k=1..n-1} r(k)*a(n-k), a(0) = 0, where r = A000081:
 *            r(1) = 1, r(m) = (1/(m-1)) * Sum_{k=1..m-1} s(k)*r(m-k), s(k) =
 *            Sum_{d|k} d*r(d)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A107_MAXN 10010

static seq_t A107_R[A107_MAXN];
static seq_t A107_S[A107_MAXN];
static seq_t A107_A[A107_MAXN];
static idx_t A107_done = 0;

static void a107_extend(idx_t n)
{
    idx_t m, k, d;
    for (m = A107_done + 1; m <= n; m++) {
        if (m == 1) {
            A107_R[1] = 1;
            A107_S[1] = 1;
            A107_A[1] = 1;
            continue;
        }
        /* r(m) = (Sum_{k=1..m-1} s(k) r(m-k)) / (m-1), tracking wrap-around carries */
        {
            seq_t S = 0, carry = 0, q, rem;
            idx_t nn = m - 1;
            for (k = 1; k < m; k++) {
                seq_t t = A107_S[k] * A107_R[m - k];
                seq_t old = S;
                S += t;
                if (S < old) carry++;
            }
            q = S / nn;
            rem = S % nn;
            if (carry > 0) {
                seq_t M = (seq_t)0 - 1;          /* 2^w - 1 */
                seq_t Q = M / nn;
                seq_t Rr = M % nn + 1;           /* 2^w = Q*nn + Rr */
                q += carry * Q;
                rem += carry * Rr;
                q += rem / nn;
            }
            A107_R[m] = q;
        }
        /* s(m) = Sum_{d|m} d*r(d) */
        {
            seq_t s = 0;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    s += (seq_t)d * A107_R[d];
                    if (e != d) s += (seq_t)e * A107_R[e];
                }
            }
            A107_S[m] = s;
        }
        /* a(m) = r(m) + Sum_{k=1..m-1} r(k) a(m-k) */
        {
            seq_t v = A107_R[m];
            for (k = 1; k < m; k++)
                v += A107_R[k] * A107_A[m - k];
            A107_A[m] = v;
        }
    }
    if (n > A107_done) A107_done = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n >= A107_MAXN) n = A107_MAXN - 1;
    if (n > A107_done) a107_extend(n);
    return A107_A[n];
}

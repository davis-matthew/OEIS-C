/*
 * A000237  Number of mixed Husimi trees with n nodes; or rooted polygonal
 *          cacti with bridges.
 * https://oeis.org/A000237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen, nice, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n+1) = EULER(BIK(a))(n), i.e. prod_{k>=1}
 *            (1-x^k)^(-b(k)) = sum_{n>=0} a(n+1) x^n, where b = BIK(a) is
 *            b(m) = [x^m] ( (1+A(x))/(1-A(x^2)) + U(x)*S(x)/(1-A(x^2)) ),
 *            A(x) = sum a(n) x^n, S(x) = 1/(1-A(x)), U(m) = sum_{i<m-i}
 *            a(i)a(m-i) + C(a(m/2),2) (unordered unequal end pairs)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 34, u128 n <= 64,
 *            big any n
 * Helpers:   mh_ch2, mh_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mh_ch2.h"
#include "../utils/mh_gcd.h"

#define MAXN 10010
#define FCAP 150

static seq_t A[MAXN + 3], S[MAXN + 3], W[MAXN + 3], Q[MAXN + 3], Bv[MAXN + 3], D[MAXN + 3];
static seq_t E[FCAP + 3], MCv[FCAP + 3];
static long long done_n = -1;
static int mode = -1; /* 1 = fixed width, 0 = arbitrary precision */

/* computes S[m], W[m], Q[m], Bv[m] = BIK(a)(m) from A[1..m] and earlier values */
static void mh_step(long long m)
{
    long long k, i, s;
    seq_t t = 0;
    for (k = 1; k <= m; k++) t += A[k] * S[m - k];
    S[m] = t;

    t = 0;
    for (i = 1; 2 * i < m; i++) t += A[i] * A[m - i];
    if (m % 2 == 0) t += mh_ch2(A[m / 2]);
    W[m] = t;

    t = 0;
    for (s = 2; s <= m; s++) t += W[s] * S[m - s];
    for (i = 1; 2 * i <= m; i++) t += A[i] * Q[m - 2 * i];
    Q[m] = t;

    t = 0;
    if (m % 2 == 0) t += S[m / 2];
    for (k = (m % 2 == 0) ? 2 : 1; k <= m; k += 2) t += A[k] * S[(m - k) / 2];
    Bv[m] = t + Q[m];
}

static void mh_init(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 300; i++) x = x + x;
    mode = (x == 0) ? 1 : 0;
    A[0] = 0;
    A[1] = 1;
    S[0] = 1;
    Q[0] = 0;
    W[0] = 0;
    W[1] = 0;
    done_n = 1;

    if (mode == 1) {
        long long m, deg, j, N = FCAP;
        for (deg = 0; deg <= N; deg++) E[deg] = 0;
        E[0] = 1;
        for (m = 1; m < N; m++) {
            seq_t b, r;
            long long jmax;
            mh_step(m);
            b = Bv[m];
            jmax = N / m;
            r = 1;
            for (j = 1; j <= jmax; j++) {
                seq_t xx = b + (seq_t)(j - 1);
                unsigned long long xr = (unsigned long long)(xx % (seq_t)j);
                unsigned long long g = mh_gcd((unsigned long long)j, xr);
                r = (r / (seq_t)((unsigned long long)j / g)) * (xx / (seq_t)g);
                MCv[j] = r;
            }
            for (deg = N; deg >= m; deg--) {
                seq_t acc = 0;
                for (j = 1; j * m <= deg; j++) acc += MCv[j] * E[deg - j * m];
                E[deg] += acc;
            }
            A[m + 1] = E[m];
        }
        done_n = N;
    }
}

seq_t a(idx_t n)
{
    if (mode < 0) mh_init();
    if (n == 0) return 0;
    if (mode == 1) {
        if (n > (idx_t)FCAP) return 0;
        return A[n];
    }
    if (n > (idx_t)MAXN) return 0;
    while (done_n < (long long)n) {
        long long m = done_n, e, k;
        seq_t s = 0;
        mh_step(m);
        for (e = 1; e <= m; e++)
            if (m % e == 0) s += (seq_t)e * Bv[e];
        D[m] = s;
        s = 0;
        for (k = 1; k <= m; k++) s += D[k] * A[m - k + 1];
        A[m + 1] = s / (seq_t)m;
        done_n = m + 1;
    }
    return A[n];
}

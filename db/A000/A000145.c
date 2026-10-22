/*
 * A000145  Number of ways of writing n as a sum of 12 squares.
 * https://oeis.org/A000145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: theta_3(q)^12 = (Sum_k x^k^2)^12; a(n) = Sum_{k=0..n}
 *            r6(k)*r6(n-k), where r6 = coefficients of theta_3(q)^6, built by
 *            multiplying by theta_3 six times
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 56, u64 n <= 4673,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A145_MAXN 65536

static seq_t a145_r6[A145_MAXN];
static seq_t a145_t1[A145_MAXN];
static seq_t a145_t2[A145_MAXN];
static idx_t a145_built = 0;

static void a145_build(idx_t N)
{
    idx_t i, m, x;
    int step;
    seq_t *cur = a145_t1, *nxt = a145_t2, *tmp;
    for (i = 0; i < N; i++) cur[i] = 0;
    cur[0] = 1;
    for (step = 0; step < 6; step++) {
        for (m = 0; m < N; m++) {
            seq_t s = cur[m];
            for (x = 1; x * x <= m; x++)
                s += cur[m - x * x] * 2;
            nxt[m] = s;
        }
        tmp = cur; cur = nxt; nxt = tmp;
    }
    for (i = 0; i < N; i++) a145_r6[i] = cur[i];
    a145_built = N;
}

seq_t a(idx_t n)
{
    idx_t k;
    seq_t s = 0;
    if (n >= A145_MAXN) return 0;
    if (n + 1 > a145_built) {
        idx_t N = 2 * (n + 1);
        if (N > A145_MAXN) N = A145_MAXN;
        a145_build(N);
    }
    for (k = 0; k <= n; k++)
        s += a145_r6[k] * a145_r6[n - k];
    return s;
}

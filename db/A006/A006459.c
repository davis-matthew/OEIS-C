/*
 * A006459  Number of elements in Z[ sqrt(-2) ] whose 'smallest algorithm' is
 *          <= n.
 * https://oeis.org/A006459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   no closed form or recurrence is known; computed by Motzkin's
 *            construction. A_0 = {0}, and A_n = A_{n-1} together with every
 *            nonzero b in Z[sqrt(-2)] for which each residue class mod b
 *            contains an element of A_{n-1}. Then a(n) = |A_n|
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   isqrt_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isqrt_i64.h"

static long long A6459_egcd(long long a, long long b, long long *u, long long *v)
{
    /* returns g = gcd(a,b) >= 0 with u*a + v*b = g, for a,b >= 0 */
    long long r0 = a, r1 = b, s0 = 1, s1 = 0, t0 = 0, t1 = 1;
    while (r1 != 0) {
        long long qq = r0 / r1, tmp;
        tmp = r0 - qq * r1; r0 = r1; r1 = tmp;
        tmp = s0 - qq * s1; s0 = s1; s1 = tmp;
        tmp = t0 - qq * t1; t0 = t1; t1 = tmp;
    }
    *u = s0; *v = t0;
    return r0;
}

static long long *A6459_cnt = NULL;
static idx_t A6459_ncnt = 0, A6459_cntcap = 0;
static int *A6459_sx = NULL, *A6459_sy = NULL;
static long long A6459_sm = 0, A6459_scap = 0;
static unsigned char *A6459_grid = NULL;
static long long A6459_R = -1;
static int *A6459_stamp = NULL;
static long long A6459_stcap = 0;
static int A6459_cur = 0;

static void A6459_set(long long x, long long y)
{
    long long W = 2 * A6459_R + 1;
    A6459_grid[(x + A6459_R) * W + (y + A6459_R)] = 1;
}

static int A6459_in(long long x, long long y)
{
    long long W = 2 * A6459_R + 1;
    if (x < -A6459_R || x > A6459_R || y < -A6459_R || y > A6459_R) return 0;
    return A6459_grid[(x + A6459_R) * W + (y + A6459_R)];
}

static void A6459_ensure_grid(long long m)
{
    long long need = isqrt_i64(m) + 2, i, W;
    if (need <= A6459_R) return;
    A6459_R = need * 2;
    W = 2 * A6459_R + 1;
    free(A6459_grid);
    A6459_grid = (unsigned char *)calloc((size_t)(W * W), 1);
    for (i = 0; i < A6459_sm; i++) A6459_set(A6459_sx[i], A6459_sy[i]);
}

static void A6459_push(int x, int y)
{
    if (A6459_sm == A6459_scap) {
        A6459_scap = A6459_scap ? 2 * A6459_scap : 64;
        A6459_sx = (int *)realloc(A6459_sx, (size_t)A6459_scap * sizeof(int));
        A6459_sy = (int *)realloc(A6459_sy, (size_t)A6459_scap * sizeof(int));
    }
    A6459_sx[A6459_sm] = x;
    A6459_sy[A6459_sm] = y;
    A6459_sm++;
}

/* does every residue class mod p+q*sqrt(-2) (p,q >= 0) meet the current set S? */
static int A6459_test(long long p, long long q, long long m)
{
    long long N = p * p + 2 * q * q, g, u, v, h21, h11, cov = 0, i;
    if (N > m) return 0;
    if (N == 1) return 1;
    /* lattice basis rows (p,q), (-2q,p); y-components q,p */
    g = A6459_egcd(q, p, &u, &v);
    h21 = u * p - 2 * v * q;
    h11 = N / g;
    A6459_cur++;
    for (i = 0; i < m; i++) {
        long long x = A6459_sx[i], y = A6459_sy[i], k, yr, xr, idx;
        k = y / g;
        if (y - k * g < 0) k--;
        yr = y - k * g;
        xr = (x - k * h21) % h11;
        if (xr < 0) xr += h11;
        idx = yr * h11 + xr;
        if (A6459_stamp[idx] != A6459_cur) {
            A6459_stamp[idx] = A6459_cur;
            cov++;
            if (cov == N) return 1;
        }
        if (m - 1 - i < N - cov) return 0;
    }
    return 0;
}

static void A6459_next_level(void)
{
    long long m = A6459_sm, x, y, xmax, nn = 0, i;
    int *nx, *ny;
    A6459_ensure_grid(m);
    if (A6459_stcap < m + 1) {
        A6459_stcap = 2 * (m + 1);
        free(A6459_stamp);
        A6459_stamp = (int *)calloc((size_t)A6459_stcap, sizeof(int));
        A6459_cur = 0;
    }
    xmax = isqrt_i64(m);
    nx = (int *)malloc((size_t)(m + 4) * sizeof(int));
    ny = (int *)malloc((size_t)(m + 4) * sizeof(int));
    for (x = 0; x <= xmax; x++) {
        long long ymax = isqrt_i64((m - x * x) / 2);
        for (y = 0; y <= ymax; y++) {
            if (x == 0 && y == 0) continue;
            if (A6459_in(x, y)) continue;
            if (A6459_test(x, y, m)) {
                nx[nn] = (int)x; ny[nn] = (int)y; nn++;
            }
        }
    }
    for (i = 0; i < nn; i++) {
        int px = nx[i], py = ny[i];
        A6459_push(px, py);
        if (px != 0) A6459_push(-px, py);
        if (py != 0) A6459_push(px, -py);
        if (px != 0 && py != 0) A6459_push(-px, -py);
    }
    free(nx); free(ny);
    A6459_ensure_grid(A6459_sm);
    for (i = m; i < A6459_sm; i++) A6459_set(A6459_sx[i], A6459_sy[i]);
}

seq_t a(idx_t n)
{
    if (A6459_ncnt == 0) {
        A6459_push(0, 0);
        A6459_ensure_grid(1);
        A6459_set(0, 0);
        A6459_cntcap = 64;
        A6459_cnt = (long long *)malloc((size_t)A6459_cntcap * sizeof(long long));
        A6459_cnt[0] = 1;
        A6459_ncnt = 1;
    }
    while (A6459_ncnt <= n) {
        A6459_next_level();
        if (A6459_ncnt == A6459_cntcap) {
            A6459_cntcap *= 2;
            A6459_cnt = (long long *)realloc(A6459_cnt, (size_t)A6459_cntcap * sizeof(long long));
        }
        A6459_cnt[A6459_ncnt++] = A6459_sm;
    }
    return (seq_t)A6459_cnt[n];
}

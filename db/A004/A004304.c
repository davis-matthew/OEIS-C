/*
 * A004304  Number of nonseparable planar tree-rooted maps with n edges.
 * https://oeis.org/A004304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = [x/Series_Reversion(x*F(x)^2)]^(1/2), F(x) = Sum_{n>=0}
 *            C(n)*C(n+1)*x^n (g.f. of A005568); equivalently A(x*F(x)^2) =
 *            F(x), so with y = x*F(x)^2: a(n) = A005568(n) - Sum_{k=1..n-1}
 *            a(k)*[x^n] y^k, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static void compute(idx_t N)
{
    idx_t i, j, k, m;
    seq_t *C = alloc_zero(N + 2);
    seq_t *F = alloc_zero(N + 1);
    seq_t *y = alloc_zero(N + 1);
    seq_t *S = alloc_zero(N + 1);
    seq_t *row = alloc_zero(N + 1);
    seq_t *nxt = alloc_zero(N + 1);
    seq_t *res = alloc_zero(N + 1);
    seq_t *tmp;

    /* Catalan numbers by convolution (no division) */
    C[0] = 1;
    for (m = 0; m <= N; m++) {
        seq_t s = 0;
        for (i = 0; i <= m; i++) s += C[i] * C[m - i];
        C[m + 1] = s;
    }
    for (i = 0; i <= N; i++) F[i] = C[i] * C[i + 1];

    /* y = x*F(x)^2 */
    for (j = 1; j <= N; j++) {
        seq_t s = 0;
        for (i = 0; i <= j - 1; i++) s += F[i] * F[j - 1 - i];
        y[j] = s;
    }

    res[0] = F[0];
    for (m = 1; m <= N; m++) row[m] = y[m];   /* row = y^1 */

    for (k = 1; k <= N; k++) {
        res[k] = F[k] - S[k];
        for (m = k + 1; m <= N; m++) S[m] += res[k] * row[m];
        if (k < N) {
            /* nxt = row * y, degrees k+1..N */
            for (m = k + 1; m <= N; m++) {
                seq_t s = 0;
                for (j = 1; j <= m - k; j++) s += y[j] * row[m - j];
                nxt[m] = s;
            }
            tmp = row; row = nxt; nxt = tmp;
        }
    }

    release(C, N + 2);
    release(F, N + 1);
    release(y, N + 1);
    release(S, N + 1);
    release(row, N + 1);
    release(nxt, N + 1);
    if (memo) release(memo, memo_len);
    memo = res;
    memo_len = N + 1;
}

seq_t a(idx_t n)
{
    if (n >= memo_len) {
        idx_t target = n;
        if (memo_len > 0 && 2 * memo_len > target) target = 2 * memo_len;
        if (target < 32) target = 32;
        compute(target);
    }
    return memo[n];
}

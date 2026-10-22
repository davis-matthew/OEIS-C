/*
 * A005805  Number of phylogenetic trees with n labels.
 * https://oeis.org/A005805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} Stirling2(n,k) * b(k), where b(1) = 1 and
 *            b(k) = A000311(k-1) for k >= 2; A000311: t(0) = 0, t(1) = 1,
 *            t(2) = 1, t(m+1) = (m+2)*t(m) + 2*Sum_{k=2..m-1}
 *            binomial(m,k)*t(k)*t(m-k+1) for m >= 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *a005805_memo = NULL;
static idx_t a005805_lim = 0;

static void a005805_build(idx_t N) {
    seq_t *t = alloc_zero(N + 2);
    seq_t *p = alloc_zero(N + 2);
    seq_t *s = alloc_zero(N + 2);
    seq_t *res = alloc_zero(N + 2);
    idx_t m, j, k;

    /* A000311 values t[0..N] */
    t[1] = 1;
    if (N >= 2) t[2] = 1;
    p[0] = 1; /* Pascal row 0 */
    for (m = 1; m + 1 <= N; m++) {
        for (j = m; j >= 1; j--) p[j] += p[j - 1];
        if (m >= 2) {
            seq_t sum = 0;
            for (k = 2; k + 1 <= m; k++) {
                sum += p[k] * t[k] * t[m - k + 1];
            }
            t[m + 1] = (seq_t)(m + 2) * t[m] + sum + sum;
        }
    }

    /* Stirling transform of b */
    s[0] = 1; /* Stirling2 row 0 */
    for (m = 1; m <= N; m++) {
        seq_t total = 0;
        for (k = m; k >= 1; k--) {
            s[k] = (seq_t)k * s[k] + s[k - 1];
        }
        s[0] = 0;
        total += s[1]; /* b(1) = 1 */
        for (k = 2; k <= m; k++) {
            total += s[k] * t[k - 1];
        }
        res[m] = total;
    }

    release(t, N + 2);
    release(p, N + 2);
    release(s, N + 2);
    if (a005805_memo != NULL) release(a005805_memo, a005805_lim + 2);
    a005805_memo = res;
    a005805_lim = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (a005805_memo == NULL || n > a005805_lim) {
        idx_t N = 16;
        if (a005805_lim * 2 > N) N = a005805_lim * 2;
        if (n > N) N = n;
        a005805_build(N);
    }
    return a005805_memo[n];
}

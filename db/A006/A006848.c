/*
 * A006848  Number of extreme points of the set of n X n symmetric
 *          doubly-substochastic matrices.
 * https://oeis.org/A006848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. f(x) satisfies f'/f = 1 + x + 1/(1-x^2)^2, so with m =
 *            n-1: a(n) = 2*a(m) + m*(2m-1)*a(m-1) - 2*m*(m-1)*a(m-2) -
 *            m*(m-1)^2*(m-2)*a(m-3) + m*(m-1)*(m-2)*(m-3)*a(m-4) +
 *            m*(m-1)*(m-2)*(m-3)*(m-4)*a(m-5), a(0) = 1 (terms with negative
 *            index are 0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t len = 0; /* number of computed terms */
    static idx_t cap = 0;

    if (memo == NULL) {
        cap = 16;
        memo = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < cap; i++) memo[i] = 0;
        memo[0] = 1;
        len = 1;
    }
    if (n < len) return memo[n];

    if (n + 1 > cap) {
        idx_t nc = cap;
        while (nc < n + 1) nc *= 2;
        seq_t *nm = (seq_t *)malloc(nc * sizeof(seq_t));
        for (idx_t i = 0; i < nc; i++) nm[i] = 0;
        for (idx_t i = 0; i < len; i++) nm[i] = memo[i];
        free(memo);
        memo = nm;
        cap = nc;
    }

    for (idx_t k = len; k <= n; k++) {
        idx_t m = k - 1;
        seq_t sm = (seq_t)m;
        seq_t pos = (seq_t)2 * memo[m];
        seq_t neg = 0;
        if (m >= 1) {
            pos += sm * ((seq_t)2 * sm - 1) * memo[m - 1];
        }
        if (m >= 2) {
            neg += (seq_t)2 * sm * (sm - 1) * memo[m - 2];
        }
        if (m >= 3) {
            neg += sm * (sm - 1) * (sm - 1) * (sm - 2) * memo[m - 3];
        }
        if (m >= 4) {
            pos += sm * (sm - 1) * (sm - 2) * (sm - 3) * memo[m - 4];
        }
        if (m >= 5) {
            pos += sm * (sm - 1) * (sm - 2) * (sm - 3) * (sm - 4) * memo[m - 5];
        }
        memo[k] = pos - neg;
    }
    len = n + 1;
    return memo[n];
}

/*
 * A005494  3-Bell numbers: E.g.f.: exp(3*z + exp(z) - 1).
 * https://oeis.org/A005494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), where T is the 3-Stirling-like
 *            triangle T(0,0) = 1, T(n+1,k) = T(n,k-1) + (k+3)*T(n,k) (row
 *            sums of the Sheffer triangle (exp(3*x), exp(x)-1))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 39,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < memo_len) return memo[n];

    idx_t len = n + 1;
    seq_t *row = alloc_zero(len + 1);
    seq_t *res = alloc_zero(len);

    row[0] = 1;
    res[0] = 1;
    for (idx_t m = 1; m <= n; m++) {
        for (idx_t k = m; k >= 1; k--) {
            row[k] = row[k - 1] + (seq_t)(k + 3) * row[k];
        }
        row[0] = row[0] * 3;
        seq_t s = 0;
        for (idx_t k = 0; k <= m; k++) {
            s += row[k];
        }
        res[m] = s;
    }

    release(row, len + 1);
    if (memo != NULL) release(memo, memo_len);
    memo = res;
    memo_len = len;
    return memo[n];
}

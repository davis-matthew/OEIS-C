/*
 * A006680  Number of binary phylogenetic trees with n labels.
 * https://oeis.org/A006680
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-2)/2)} n!/(n-k)! * binomial(m,k) *
 *            (2m-1)!!, where m = n-2-k. This counts trees whose vertices all
 *            have degree 1 or 3, with every leaf labeled and k internal
 *            vertices also labeled. Consecutive terms satisfy T(k+1) = T(k) *
 *            (n-k)(m-k)(m-k-1) / ((k+1) m (2m-1)), with T(0)...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 19 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t ratio_sum(idx_t n)
{
    idx_t kmax = (n - 2) / 2, k, i;
    seq_t t = 1, s;
    for (i = 1; i + 2 <= n; i++) t *= (seq_t)(2 * i - 1);
    s = t;
    for (k = 0; k < kmax; k++) {
        uint64_t m = n - 2 - k;
        uint64_t num = (uint64_t)(n - k) * (m - k) * (m - k - 1);
        uint64_t den = (uint64_t)(k + 1) * m * (2 * m - 1);
        t = t * (seq_t)num / (seq_t)den;
        s += t;
    }
    return s;
}

static seq_t direct_sum(idx_t n)
{
    seq_t s = 0;
    idx_t kmax = (n - 2) / 2, k, i;
    for (k = 0; k <= kmax; k++) {
        idx_t m = n - 2 - k;
        seq_t t = 1;
        for (i = 0; i < k; i++) t *= (seq_t)(n - i);
        t *= binomial(m, k);
        for (i = 1; i <= m; i++) t *= (seq_t)(2 * i - 1);
        s += t;
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (seq_is_big()) return ratio_sum(n);
    return direct_sum(n);
}

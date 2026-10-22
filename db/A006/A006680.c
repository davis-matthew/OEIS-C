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
 * Helpers:   binom, ratio_sum, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/seq_is_big.h"
#include "../utils/ratio_sum.h"

static seq_t direct_sum(idx_t n)
{
    seq_t s = 0;
    idx_t kmax = (n - 2) / 2, k, i;
    for (k = 0; k <= kmax; k++) {
        idx_t m = n - 2 - k;
        seq_t t = 1;
        for (i = 0; i < k; i++) t *= (seq_t)(n - i);
        t *= binom(m, k);
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

/*
 * A003704  Expansion of log(1+sinh(x)).
 * https://oeis.org/A003704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   (1+sinh(x))*f'(x) = cosh(x), where f = log(1+sinh(x)), gives
 *            a(n+1) = [n even] - Sum_{k odd, 1<=k<=n} binomial(n,k)*a(n+1-k),
 *            a(0) = 0, a(1) = 1
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_arr.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n < memo_len) return memo[n];
    {
        idx_t newlen = n + 1;
        idx_t start = memo_len;
        memo = grow_arr(memo, memo_len, newlen);
        memo_len = newlen;
        if (start == 0) {
            memo[0] = 0;
            start = 1;
        }
        for (idx_t t = start; t < newlen; t++) {
            if (t == 1) { memo[1] = 1; continue; }
            idx_t m = t - 1;
            seq_t s = (m % 2 == 0) ? (seq_t)1 : (seq_t)0;
            seq_t c = 1; /* binomial(m, k) */
            for (idx_t k = 1; k <= m; k++) {
                c = c * (seq_t)(m - k + 1) / (seq_t)k;
                if (k % 2 == 1) s -= c * memo[m + 1 - k];
            }
            memo[t] = s;
        }
    }
    return memo[n];
}

/*
 * A007219  Number of golygons of order 8n (or serial isogons of order 8n).
 * https://oeis.org/A007219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (P/2)*(Q/2), where m = 4n, P = number of sign choices
 *            with +-1 +-3 ... +-(2m-1) = 0 (counted by subset-sum DP: subsets
 *            of {1,3,...,2m-1} summing to m^2/2), Q = number of sign choices
 *            with +-1 +-2 ... +-m = 0 (subsets of {1,...,m} summing to
 *            m(m+1)/4). a(n) = A006718(n)/4 = P*Q/4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* number of subsets of {v_1..v_m} (v_i = 2i-1 if odd, else i) summing to half the total */
static seq_t half_subsets(idx_t m, int odd)
{
    idx_t total = odd ? m * m : m * (m + 1) / 2;
    if (total % 2) return (seq_t)0;
    idx_t T = total / 2;
    seq_t *dp = alloc_zero(T + 1);
    dp[0] = (seq_t)1;
    idx_t reach = 0;
    for (idx_t i = 1; i <= m; i++) {
        idx_t v = odd ? 2 * i - 1 : i;
        reach += v;
        idx_t hi = reach < T ? reach : T;
        for (idx_t s = hi; s >= v; s--)
            dp[s] += dp[s - v];
    }
    seq_t r = dp[T];
    release(dp, T + 1);
    return r;
}

seq_t a(idx_t n)
{
    idx_t m = 4 * n;
    seq_t p = half_subsets(m, 1);
    seq_t q = half_subsets(m, 0);
    return (p / 2) * (q / 2);
}

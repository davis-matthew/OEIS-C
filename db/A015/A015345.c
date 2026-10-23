/*
 * A015345  Gaussian binomial coefficient [ n,7 ] for q = -6.
 * https://oeis.org/A015345
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,7] with q = -6
 *            Recurrence: C(n, k) = C(n-1, k) + q^(n-k) * C(n-1, k-1) (k = 7).
 *            See OEIS A015345
 * Range:     i8 n <= 7, i16 n <= 7, i32 n <= 8, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n < 7 the sequence is undefined; return 0. */
    if (n < 7)
        return 0;

    /* dp[j] holds the Gaussian binomial [current_n, j] */
    seq_t dp[8] = {0};
    dp[0] = 1;  /* By definition [n,0] = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t up = i < 7 ? i : 7;
        for (idx_t j = up; j >= 1; --j) {
            seq_t powq = big_pow((seq_t)-6, (seq_t)(i - j));
            dp[j] = dp[j] + powq * dp[j - 1];
        }
    }

    return dp[7];
}

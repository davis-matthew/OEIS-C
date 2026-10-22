/*
 * A001469  Genocchi numbers (of first kind); unsigned coefficients give
 *          expansion of x*tan(x/2).
 * https://oeis.org/A001469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   Seidel's recurrence: for n>1, 0 = Sum_{j=0..floor(n/2)}
 *            C(n,2j)*a(n-j)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 15, i128 n <= 24,
 *            big any n
 * Helpers:   a000707_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a000707_binom.h"

seq_t a(idx_t n) {
    if (n == 1) return -1;
    
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    dp[1] = -1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= i / 2; j++) {
            sum += a000707_binom(i, 2 * j) * dp[i - j];
        }
        dp[i] = -sum;
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}

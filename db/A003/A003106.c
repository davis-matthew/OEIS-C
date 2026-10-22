/*
 * A003106  Number of partitions of n into parts 5k+2 or 5k+3.
 * https://oeis.org/A003106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: Product_{k>=1} 1/((1-x^(5k-2))*(1-x^(5k-3))), i.e. a(n) =
 *            number of partitions of n into parts congruent to 2 or 3 mod 5
 * Range:     u8 n <= 40, u16 n <= 104, u32 n <= 308, u64 n <= 1010,
 *            u128 n <= 3565, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t result;
    idx_t part, i;

    dp[0] = 1;
    for (part = 2; part <= n; ++part) {
        int mod5 = (int)(part % 5);
        if (mod5 != 2 && mod5 != 3) continue;
        for (i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    result = dp[n];
    free(dp);
    return result;
}

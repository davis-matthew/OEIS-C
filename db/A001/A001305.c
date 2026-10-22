/*
 * A001305  Expansion of 1/((1-x)^2*(1-x^2)*(1-x^5)*(1-x^10)*(1-x^20)).
 * https://oeis.org/A001305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: 1/((1-x)^2*(1-x^2)*(1-x^5)*(1-x^10)*(1-x^20)); a(n) is the
 *            coefficient of x^n, computed by starting from 1/(1-x)^2
 *            (coefficient n+1) and multiplying successively by 1/(1-x^d) for
 *            d = 2, 5, 10, 20: c(i) += c(i-d)
 * Range:     u8 n <= 19, u16 n <= 90, u32 n <= 986, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/u128_to_seq.h"

seq_t a(idx_t n) {
    static const idx_t dens[4] = {2, 5, 10, 20};
    unsigned __int128 *dp;
    unsigned __int128 r;
    idx_t i;
    int j;

    dp = (unsigned __int128 *)malloc((size_t)(n + 1) * sizeof(unsigned __int128));
    if (!dp) return (seq_t)0;

    for (i = 0; i <= n; i++)
        dp[i] = (unsigned __int128)i + 1;

    for (j = 0; j < 4; j++) {
        idx_t d = dens[j];
        for (i = d; i <= n; i++)
            dp[i] += dp[i - d];
    }

    r = dp[n];
    free(dp);
    return u128_to_seq(r);
}

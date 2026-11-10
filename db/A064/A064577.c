/*
 * A064577  First differences of A064574.
 * https://oeis.org/A064577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = A064574(n+1) - A064574(n), where A064574(m) =
 *            Sum_{composite c<=m} (number of partitions of m into powers of
 *            c, minus 1); computed by sieve + coin-change DP
 * Range:     u8 n <= 526, u16 n <= 3838, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 115 terms through n = 9826 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A064574_f(idx_t m)
{
    if (m < 4) return 0;
    unsigned char *comp = (unsigned char *)malloc((size_t)(m + 1));
    for (idx_t i = 0; i <= m; ++i) comp[i] = 0;
    for (idx_t i = 2; i * i <= m; ++i)
        if (!comp[i])
            for (idx_t j = i * i; j <= m; j += i) comp[j] = 1;

    seq_t total = 0;
    seq_t *dp = mem_new(seq_t, (size_t)(m + 1));

    for (idx_t c = 4; c <= m; ++c) {
        if (!comp[c]) continue;
        if (c > m / c) {
            /* only powers 1 and c: floor(m/c)+1 partitions, minus 1 */
            total += (seq_t)(m / c);
            continue;
        }
        idx_t powers[70];
        idx_t np = 0, val = 1;
        while (val <= m) {
            powers[np++] = val;
            if (val > m / c) break;
            val *= c;
        }
        for (idx_t i = 0; i <= m; ++i) dp[i] = 0;
        dp[0] = 1;
        for (idx_t p = 0; p < np; ++p) {
            idx_t v = powers[p];
            for (idx_t s = v; s <= m; ++s) dp[s] += dp[s - v];
        }
        total += dp[m] - 1;
    }
    mem_free(dp);
    free(comp);
    return total;
}

seq_t a(idx_t n)
{
    return A064574_f(n + 1) - A064574_f(n);
}

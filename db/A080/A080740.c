/*
 * A080740  a(n) = number of m such that A080737(m) <= 2n.
 * https://oeis.org/A080740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of m with A080737(m) <= 2n, where A080737(m) is
 *            the sum over prime powers p^e || m of cost(p^e): cost(2)=0,
 *            cost(2^e)=2^(e-1) for e>1, cost(p^e)=p^(e-1)*(p-1) for odd p,
 *            and cost(1)=0. Computed by a grouped knapsack DP over primes:
 *            each prime contributes at most one power, and the D...
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t L = (size_t)(2 * n);
    seq_t *dp = mem_new(seq_t, L + 1);
    dp[0] = 1;
    size_t lim = L + 2;
    unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);

    for (size_t p = 2; p <= lim; ++p)
    {
        if (comp[p]) continue;
        for (size_t q = p * p; q <= lim; q += p) comp[q] = 1;

        /* option costs for this prime */
        size_t costs[64];
        int nc = 0;
        int extra0 = 0;
        if (p == 2)
        {
            extra0 = 1; /* exponent 1 also costs 0 */
            size_t c = 2;
            while (c <= L && nc < 60) { costs[nc++] = c; c *= 2; }
        }
        else
        {
            size_t c = p - 1;
            while (c <= L && nc < 60) { costs[nc++] = c; c *= p; }
        }
        if (nc == 0 && !extra0) continue;
        for (size_t c = L + 1; c-- > 0;)
        {
            seq_t v = dp[c];
            if (extra0) v = v * 2;
            for (int i = 0; i < nc; ++i)
            {
                if (costs[i] > c) break;
                v += dp[c - costs[i]];
            }
            dp[c] = v;
        }
    }

    seq_t total = 0;
    for (size_t c = 0; c <= L; ++c) total += dp[c];
    free(comp);
    mem_free(dp);
    return total;
}

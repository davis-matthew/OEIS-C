/*
 * A085491  Number of ways to write n as sum of distinct divisors of n+1.
 * https://oeis.org/A085491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a sum of * distinct divisors
 *            of n+1. * This is done by collecting all divisors d ≤ n of (n+1)
 *            * and then a classic subset‑sum DP: dp[s] = number of ways * to
 *            form sum s using the already processed divisors. * dp[0] is 1
 *            (empty sum)
 * Range:     u8 n <= 118, u16 n <= 718, u32 n <= 2518, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1. Collect divisors of n+1 that are ≤ n */
    idx_t m = n + 1;
    size_t cap = 16, cnt = 0;
    idx_t *divs = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!divs) return 0;            /* allocation failure */

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            if (d <= n) {
                if (cnt == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t*)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[cnt++] = d;
            }
            idx_t e = m / d;
            if (e != d && e <= n) {
                if (cnt == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t*)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[cnt++] = e;
            }
        }
    }

    /* 2. Subset‑sum DP */
    size_t size = (size_t)n + 1;                /* n fits in size_t for  n ≤ 10000 */
    seq_t *dp = (seq_t*)malloc(size * sizeof(seq_t));
    if (!dp) { free(divs); return 0; }

    for (size_t i = 0; i < size; ++i) dp[i] = 0;
    dp[0] = 1;                                 /* empty set */

    for (size_t k = 0; k < cnt; ++k) {
        idx_t d = divs[k];
        for (idx_t s = n; s >= d; --s) {
            dp[s] += dp[s - d];
            if (s == d) break;                /* avoid underflow of unsigned */
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(divs);
    return result;
}

/*
 * A100346  Number of compositions of n into divisors of n.
 * https://oeis.org/A100346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n>0, a(n)=Σ_{d|n} a(n−d) (order of summands matters)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 71, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* collect divisors of n */
    size_t divcap = 16;
    idx_t *divs = (idx_t *)malloc(divcap * sizeof(idx_t));
    if (!divs) return (seq_t)0;
    size_t divcnt = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if (divcnt == divcap) {
                divcap <<= 1;
                idx_t *tmp = (idx_t *)realloc(divs, divcap * sizeof(idx_t));
                if (!tmp) { free(divs); return (seq_t)0; }
                divs = tmp;
            }
            divs[divcnt++] = d;
            idx_t other = n / d;
            if (other != d) {
                if (divcnt == divcap) {
                    divcap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(divs, divcap * sizeof(idx_t));
                    if (!tmp) { free(divs); return (seq_t)0; }
                    divs = tmp;
                }
                divs[divcnt++] = other;
            }
        }
    }

    /* dynamic programming: dp[s] = number of compositions of s */
    size_t dp_size = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
    if (!dp) { free(divs); return (seq_t)0; }
    dp[0] = (seq_t)1;

    for (size_t s = 1; s < dp_size; ++s) {
        seq_t acc = 0;
        for (size_t i = 0; i < divcnt; ++i) {
            idx_t d = divs[i];
            if ((size_t)d > s) continue;
            acc += dp[s - (size_t)d];
        }
        dp[s] = acc;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(divs);
    return result;
}

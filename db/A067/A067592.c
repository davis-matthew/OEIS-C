/*
 * A067592  Number of partitions of n into Lucas parts (A000204(k) for k >=
 *          1).
 * https://oeis.org/A067592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 129, u32 n <= 723, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of partitions of n into parts from the Lucas sequence
   1, 3, 4, 7, 11, 18, … using standard dynamic programming. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* generate Lucas numbers up to n (starting with 1 and 3) */
    int lucas[64];          /* enough for n <= 10 000 (≈ 20 terms) */
    size_t lucas_len = 0;
    if (n >= 1) {
        lucas[lucas_len++] = 1;
    }
    if (n >= 3) {
        lucas[lucas_len++] = 3;
    }
    int a0 = 1, a1 = 3;      /* a0 = L1, a1 = L2 */
    while (1) {
        int next = a0 + a1;
        if ((idx_t)next > n) {
            break;
        }
        lucas[lucas_len++] = next;
        a0 = a1;
        a1 = next;
    }

    /* dynamic programming for restricted partitions */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;                     /* out-of-memory guard */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;                             /* empty partition */

    for (size_t k = 0; k < lucas_len; ++k) {
        idx_t part = (idx_t)lucas[k];
        for (idx_t j = part; j <= n; ++j) {
            dp[j] += dp[j - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}

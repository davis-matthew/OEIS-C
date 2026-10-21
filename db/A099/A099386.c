/*
 * A099386  Number of partitions of 2*n into distinct minimal numbers.
 * https://oeis.org/A099386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A099386:
   partitions of 2*n into distinct minimal numbers.
   Minimal number of d divisors is the smallest integer with exactly d divisors.
   Algorithm:
   1. Compute divisor counts for all integers ≤ 2*n.
   2. Record, for each divisor count d, the smallest integer having d divisors.
   3. The set of these distinct minimal numbers forms the parts.
   4. Count subsets of these parts that sum to 2*n using a classic
      subset‑sum DP (distinct parts).
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1. target sum */
    idx_t T = (idx_t)2 * n;
    size_t TT = (size_t)T;

    /* 2. divisor counts for 1..T */
    uint16_t *divcnt = (uint16_t*)malloc((TT + 1) * sizeof(uint16_t));
    if (!divcnt) return 0;
    for (size_t i = 0; i <= TT; ++i) divcnt[i] = 0;
    for (size_t i = 1; i <= TT; ++i) {
        for (size_t j = i; j <= TT; j += i)
            ++divcnt[j];
    }

    /* 3. minimal number for each divisor count */
    uint64_t *min_num = (uint64_t*)malloc((TT + 1) * sizeof(uint64_t));
    if (!min_num) { free(divcnt); return 0; }
    for (size_t i = 0; i <= TT; ++i) min_num[i] = 0;
    for (size_t m = 1; m <= TT; ++m) {
        uint16_t d = divcnt[m];
        if (min_num[d] == 0 || m < min_num[d])
            min_num[d] = m;
    }

    /* 4. collect distinct minimal numbers */
    size_t part_cnt = 0;
    for (size_t d = 1; d <= TT; ++d)
        if (min_num[d] != 0)
            ++part_cnt;

    uint64_t *parts = (uint64_t*)malloc(part_cnt * sizeof(uint64_t));
    if (!parts) { free(divcnt); free(min_num); return 0; }
    size_t pos = 0;
    for (size_t d = 1; d <= TT; ++d)
        if (min_num[d] != 0)
            parts[pos++] = min_num[d];

    /* 5. subset‑sum DP over the parts */
    seq_t *dp = (seq_t*)malloc((TT + 1) * sizeof(seq_t));
    if (!dp) { free(divcnt); free(min_num); free(parts); return 0; }
    for (size_t i = 0; i <= TT; ++i) dp[i] = 0;
    dp[0] = 1;

    for (size_t p = 0; p < part_cnt; ++p) {
        size_t part = (size_t)parts[p];
        if (part > TT) continue;
        for (size_t s = TT; s >= part; --s)
            dp[s] += dp[s - part];
    }

    seq_t result = dp[TT];

    /* cleanup */
    free(divcnt);
    free(min_num);
    free(parts);
    free(dp);

    return result;
}

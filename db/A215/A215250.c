/*
 * A215250  a(n) is the number of values k >= 0 such that A215244(k) = n.
 * https://oeis.org/A215250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{k >= 0 : A215244(k) = n}. Computed by a DFS over binary
 *            strings, with incremental DP over palindromic partitions.
 *            A215244 never decreases when a bit is appended, so any branch
 *            whose count exceeds n is pruned
 * Range:     u8 n <= 307, u16 n <= 307, u32 n <= 307, u64 n <= 307,
 *            u128 n <= 307, big any n
 * Validated: standard, pass, 87 terms through n = 306 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned char A215250_bits[70];
static uint64_t A215250_dp[71];
static uint64_t A215250_target;
static uint64_t A215250_count;

static void A215250_dfs(int len)
{
    int b;
    if (len >= 64) return;
    for (b = 0; b < 2; ++b) {
        int j, i;
        uint64_t s = 0;
        if (len == 0 && b == 0) continue; /* leading bit must be 1 */
        A215250_bits[len] = (unsigned char)b;
        for (j = 0; j <= len; ++j) {
            bool pal = true;
            for (i = j; i < len - (i - j); ++i) {
                if (A215250_bits[i] != A215250_bits[len - (i - j)]) { pal = false; break; }
            }
            if (pal) s += A215250_dp[j];
            if (s > A215250_target) break;
        }
        if (s > A215250_target) continue;
        A215250_dp[len + 1] = s;
        if (s == A215250_target) A215250_count++;
        A215250_dfs(len + 1);
    }
}

seq_t a(idx_t n)
{
    A215250_target = (uint64_t)n;
    A215250_count = 0;
    A215250_dp[0] = 1;
    if (n == 1) A215250_count = 1; /* k = 0 */
    A215250_dfs(0);
    return (seq_t)A215250_count;
}

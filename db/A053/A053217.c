/*
 * A053217  Numbers that do not occur in A001156.
 * https://oeis.org/A053217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer that is not a value of A001156
 *            (partitions of k into squares); computed by coin-change DP over
 *            squares, scanning gaps between consecutive (nondecreasing)
 *            values
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A053217_parts(idx_t len) {
    seq_t *dp = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) dp[i] = 0;
    dp[0] = 1;
    for (idx_t m = 1; m * m < len; m++) {
        idx_t sq = m * m;
        for (idx_t i = sq; i < len; i++) dp[i] += dp[i - sq];
    }
    return dp;
}

seq_t a(idx_t n) {
    idx_t len = 64;
    for (;;) {
        seq_t *dp = A053217_parts(len);
        seq_t rem = (seq_t)n;
        seq_t res = 0;
        bool found = false, bad = false;
        for (idx_t i = 1; i < len; i++) {
            seq_t prev = dp[i - 1], next = dp[i];
            if (next < prev) { bad = true; break; }
            if (next > prev + 1) {
                seq_t gap = next - prev - 1;
                if (gap >= rem) { res = prev + rem; found = true; break; }
                rem -= gap;
            }
        }
        mem_free(dp);
        if (found) return res;
        if (bad) return 0;
        len *= 2;
    }
}

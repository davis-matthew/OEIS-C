/*
 * A079211  Duplicate of A026835.
 * https://oeis.org/A079211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A026835(n); T(r,c) = number of partitions of r into
 *            distinct parts all >= c, where r(r-1)/2 < n <= r(r+1)/2 and c =
 *            n - r(r-1)/2
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 95 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mem.h"

static idx_t A026835_int_sqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

static seq_t A026835_a(idx_t n) {
    idx_t r = (A026835_int_sqrt_u64(8 * n + 1) - 1) / 2;
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;
    idx_t c = n - r * (r - 1) / 2;

    seq_t *dp = mem_new(seq_t, (r + 1));
    if (!dp) return 0;

    for (idx_t i = 0; i <= r; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t part = c; part <= r; ++part) {
        for (idx_t sum = r; sum >= part; --sum) {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[r];
    mem_free(dp);
    return result;
}

seq_t a(idx_t n) {
    return A026835_a(n);
}

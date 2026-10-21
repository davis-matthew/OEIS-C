/*
 * A052217  Numbers whose sum of digits is 3.
 * https://oeis.org/A052217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 220, u64 n <= 1520,
 *            u128 n <= 10000, big any n
 * Helpers:   remaining_compositions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Compute a(n) as the n‑th positive integer whose sum of
 * decimal digits equals 3.  The value is constructed by first finding
 * its digit length L from the formula for the cumulative number of
 * such integers with at most L digits:  f(L) = L(L+1)(L+2)/6.
 * Once L is known the position of the number among all L‑digit
 * solutions is enumerated lexicographically, selecting each digit
 * by counting how many completions are possible with the remaining
 * sum and positions.  This yields the exact value for any n≥1
 * without exhaustive searching. */

#include <stdint.h>
#include "../utils/remaining_compositions.h"

seq_t a(idx_t n) {
    /* Find the minimal length L such that f(L) >= n,
     * where f(L) = L(L+1)(L+2)/6. */
    idx_t L = 0;
    while (1) {
        ++L;
        __uint128_t f = (__uint128_t)L * (L + 1) * (L + 2) / 6;
        if (f >= n) break;
    }

    /* Compute the number of terms up to length L-1. */
    idx_t prev_count = 0;
    if (L > 1) {
        idx_t k = L - 1;
        prev_count = k * (k + 1) * (k + 2) / 6;
    }

    /* Index inside the block of L‑digit numbers (0‑based). */
    idx_t offset = n - prev_count - 1;

    /* Construct the number digit by digit. */
    seq_t result = 0;
    idx_t remPos = L;          /* digits remaining (including current) */
    idx_t remSum = 3;          /* remaining sum to distribute */

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t start = (pos == 0) ? 1 : 0;
        idx_t chosen = 0;
        for (idx_t d = start; d <= 9 && d <= remSum; ++d) {
            idx_t new_remSum = remSum - d;
            idx_t new_remPos = remPos - 1;
            idx_t count = remaining_compositions(new_remSum, new_remPos);
            if (offset < count) {
                chosen = d;
                break;
            } else {
                offset -= count;
            }
        }
        result = result * 10 + chosen;
        remSum -= chosen;
        remPos -= 1;
    }

    return result;
}

/*
 * A162401  a(n) is the largest integer whose square, when represented in
 *          binary, is a substring within the binary representation of n.
 * https://oeis.org/A162401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) is the largest integer whose square, when represented in
 *            binary, * is a substring of the binary representation of n
 * Range:     u8 n <= 8636, u16 n <= 8636, u32 n <= 8636, u64 n <= 8636,
 *            u128 n <= 8636, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"
#include <stdint.h>
#include <stddef.h>

/* Integer square‑root for 64‑bit values, returned as a seq_t.
 * It never uses floating point arithmetic. */
static uint64_t isqrt_ull(uint64_t x)
{
    uint64_t low = 0, high = x;
    if (high > 0xFFFFFFFFULL) high = 0xFFFFFFFFULL;   /* sqrt(2^64) < 2^32 */

    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            low = mid + 1;
        else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;   /* floor of sqrt(x) */
}

seq_t a(idx_t n)
{
    /* n is positive; the offset starts at 1 */
    if (n == 0) return (seq_t)0;   /* never reached in the sequence */

    /* Number of binary digits of n */
    size_t len = floor_log2_uint64(n) + 1;
    uint64_t best = 1;   /* at least 1 always works (1^2 = 1) */

    for (size_t i = 0; i < len; ++i) {
        /* Position of the first bit of the current substring (msb first) */
        size_t pos = len - 1 - i;
        if (((n >> pos) & 1ULL) == 0) continue;   /* must start with '1' */

        uint64_t val = 0;
        for (size_t j = i; j < len; ++j) {
            size_t pos2 = len - 1 - j;
            val = (val << 1) | ((n >> pos2) & 1ULL);

            uint64_t root = isqrt_ull(val);
            if (root * root == val && root > best)
                best = root;
        }
    }
    return (seq_t)best;
}

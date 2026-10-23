/*
 * A020646  Least positive integer k for which 7^n divides k!.
 * https://oeis.org/A020646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive k such that 7^n divides k! (i.e. the
 *            smallest k with v_7(k!) ≥ n, where v_7(k!) = Σ_{i≥1} ⌊k/7^i⌋)
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   v7_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/v7_of_uint64.h"

static inline uint64_t minimal_k(uint64_t n)
{
    if (n == 0) return 1;

    uint64_t low = 1;
    /* initial high bound: 7n + 7, but guard against overflow */
    uint64_t high;
    if (n > (UINT64_MAX - 7) / 7) {
        high = UINT64_MAX;
    } else {
        high = 7 * n + 7;
    }

    /* Ensure high is large enough for the condition */
    while (v7_of_uint64(high) < n) {
        if (high == UINT64_MAX) break;
        if (high > UINT64_MAX / 2) high = UINT64_MAX;
        else high <<= 1;
    }

    /* Binary search for minimal k */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if (v7_of_uint64(mid) >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    return (seq_t)minimal_k((uint64_t)n);
}

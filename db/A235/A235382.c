/*
 * A235382  a(n) = smallest number of unit squares required to enclose n units
 *          of area.
 * https://oeis.org/A235382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*ceil(2*sqrt(n)) + 4
 * Range:     u8 n <= 3906, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    // Compute ceil(sqrt(4*n)) using binary search on 64‑bit integers
    unsigned __int128 target = ((unsigned __int128)n) << 2;   // 4*n
    uint64_t low  = 0;
    uint64_t high = (uint64_t)1 << 33;                         // > sqrt(4*n) for any 64‑bit n

    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 mid_sq = (unsigned __int128)mid * mid;
        if (mid_sq >= target)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t m = low;   // m = ceil(2*sqrt(n))

    seq_t result = (seq_t)((seq_t)2 * (seq_t)m + 4);
    return result;
}

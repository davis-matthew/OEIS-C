/*
 * A136724  Numbers divisible by 4 that are not powers of 2.
 * https://oeis.org/A136724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * k, where k is the n‑th positive integer that is NOT a
 *            power of two (i.e.
 *            1,3,5,6,7,9,10,11,12,13,14,15,17,18,19,20,21,22,23,25,26,…)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return floor(log2(x)) for x > 0.  x==0 → 0 (unused).   */
static inline uint64_t floor_log2_u64(uint64_t x)
{
    uint64_t r = 0;
    while (x >>= 1) r++;
    return r;
}

/* Count how many positive integers ≤ x are NOT powers of two.   */
static inline uint64_t non_power_of_two_le(uint64_t x)
{
    if (x == 0) return 0;
    return x - (floor_log2_u64(x) + 1ULL);
}

/* OEIS function for sequence A136724.  */
seq_t a(idx_t n)
{
    /* Binary search for the n-th non‑power‑of‑two integer.  */
    uint64_t low  = 1;
    uint64_t high = n + 64ULL;               /* initial upper bound – safe for n ≤ 10000 */

    /* Ensure high is large enough.  */
    while (non_power_of_two_le(high) < n) {
        if (high > UINT64_MAX / 2ULL) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Classic binary search on [low, high].  */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if (non_power_of_two_le(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }

    /* Result is 4 times the found integer.  */
    return (seq_t)low * (seq_t)4;
}

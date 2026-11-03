/*
 * A032516  Sum of the integer part of 7/2-th roots of integers less than n.
 * https://oeis.org/A032516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} floor(k^(2/7))
 * Range:     u8 n <= 104, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 950 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the integer part of the (7/2)-th root of k by testing successive m
 * until m^7 > k^2, so no floating point operations are used. */
#include <stdint.h>

static inline uint64_t floor_7th_root_of_square(uint64_t k)
{
    /* Find largest m such that (m+1)^7 > k^2. Since k is up to 64‑bit the
     * value of m is small (≲ 300 000 for k = 2^64), so a linear scan is
     * inexpensive. */
    uint64_t m = 0;
    unsigned __int128 kk = (unsigned __int128)k * k;   /* k^2 */
    while (1) {
        unsigned __int128 p = 1;
        for (int i = 0; i < 7; ++i) p *= (unsigned __int128)(m + 1);
        if (p > kk) break;
        ++m;
    }
    return m;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t root = floor_7th_root_of_square((uint64_t)k);
        sum += (seq_t)root;
    }
    return sum;
}

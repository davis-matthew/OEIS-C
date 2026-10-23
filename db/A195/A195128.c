/*
 * A195128  a(n) = 2*n - floor(n*sqrt(1/2)).
 * https://oeis.org/A195128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - floor(n*sqrt(1/2)) = 2*n - floor(n/√2)
 * Range:     u8 n <= 197, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t floor_div_sqrt2(uint64_t n)
{
    /* Computes floor(n / sqrt(2)) using only 128‑bit arithmetic to avoid overflow. */
    if (n == 0) return 0;
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        unsigned __int128 rhs2 = (unsigned __int128)n * n;
        unsigned __int128 rhs_half = rhs2 >> 1;       /* floor(n*n / 2)     */
        if (mid2 <= rhs_half)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    uint64_t k = floor_div_sqrt2(n);          /* k = floor(n / sqrt(2))  */
    return (seq_t)n * (seq_t)2 - (seq_t)k;     /* a(n) = 2n - k            */
}

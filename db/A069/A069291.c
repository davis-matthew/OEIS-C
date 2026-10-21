/*
 * A069291  Number of square divisors of n <= sqrt(n).
 * https://oeis.org/A069291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8533, u16 n <= 8533, u32 n <= 8533, u64 n <= 8533,
 *            u128 n <= 8533, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069291: number of square divisors of n that do not exceed sqrt(n)
   Formula: a(n) = |{ k ∈ ℕ : 1 ≤ k ≤ floor(n^(1/4)) and k² | n }|
   (the same can be written as the number of square divisors of n that
   are ≤ sqrt(n)). */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit values.
   Binary search implementation, safe for all values up to ULLONG_MAX. */
static inline uint64_t int_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (x >> 1) + 1, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        uint64_t sq = (__int128)mid * mid;   /* use 128‑bit to avoid overflow */
        if (sq == x) return mid;
        if (sq < x) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;                 /* n as 64‑bit integer   */
    uint64_t r4 = int_sqrt_u64(int_sqrt_u64(nn)); /* floor(n^(1/4))  */

    seq_t count = 0;
    for (uint64_t k = 1; k <= r4; ++k) {
        uint64_t sq = k * k;          /* k² ≤ n, fits in 64‑bit */
        if (nn % sq == 0) {
            ++count;
        }
    }
    return count;
}

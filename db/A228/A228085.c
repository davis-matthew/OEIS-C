/*
 * A228085  a(n) = number of distinct k which satisfy n = k + wt(k), where
 *          wt(k) (A000120) gives the number of 1's in binary representation
 *          of a nonnegative integer k.
 * https://oeis.org/A228085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k such that n = k + wt(k) * where wt(k) is the
 *            number of 1's in the binary * representation of k (A000120)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned popcount_u64(idx_t x)
{
    /* use compiler intrinsic for speed (works for 64‑bit x) */
    return __builtin_popcountll((uint64_t)x);
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        if (k + (idx_t)popcount_u64(k) == n)
            ++count;
    }
    return count;
}

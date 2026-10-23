/*
 * A036489  Nearest integer to n^(7/2).
 * https://oeis.org/A036489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n^(7/2)
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n^7 using 128‑bit arithmetic to avoid overflow when
       seq_t is only 64‑bit. */
    unsigned __int128 pow7 = 1;
    for (int i = 0; i < 7; ++i)
        pow7 *= (unsigned __int128)n;

    /* Integer square root of pow7 via binary search.  The result fits
       in 64 bits, because sqrt(n^7) = n^(7/2) <= n^3.5 <= 10^14 for
       n <= 10000, well below 2^64. */
    uint64_t lo = 0, hi = (uint64_t)-1, ans = 0;
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= pow7)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }

    /* Rounding to nearest integer.  Let d = pow7 - ans^2.
       Round up if 2*d >= 2*ans + 1. */
    unsigned __int128 ans2 = (unsigned __int128)ans * ans;
    unsigned __int128 d = pow7 - ans2;
    unsigned __int128 two_d = d << 1;
    unsigned __int128 two_ans_plus_one = (unsigned __int128)ans << 1 | 1;
    if (two_d >= two_ans_plus_one)
        ++ans;

    return (seq_t)ans;
}

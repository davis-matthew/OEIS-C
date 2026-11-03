/*
 * A054071  Position of 1 in the permutation of 1,2,...,n obtained by ordering
 *          the fractional parts {h*sqrt(2)} for h=1,2,...,n.
 * https://oeis.org/A054071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ⌊n*(√2 – 1)⌋, for n ≥ 1 The integer part is computed
 *            by the equivalent inequality 2*k² ≤ (n – k)², which is solved by
 *            binary search
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t floor_n_sqrt2_minus1(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;

    /* Binary search for the maximum k such that 2*k*k ≤ (n - k)^2 */
    while (low < high) {
        idx_t diff = high - low;                     /* diff > 0 */
        idx_t mid = low + diff / 2 + 1;               /* (low+high+1)/2 */

        idx_t n_minus_mid = n - mid;
        unsigned __int128 lhs = (unsigned __int128)2 * (unsigned __int128)mid * (unsigned __int128)mid;
        unsigned __int128 rhs = (unsigned __int128)n_minus_mid * (unsigned __int128)n_minus_mid;

        if (lhs <= rhs)
            low = mid;
        else
            high = mid - 1;
    }

    return low; /* floor(n*(√2 – 1)) */
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n<1: any value is acceptable */
        return 0;

    seq_t val = (seq_t)floor_n_sqrt2_minus1(n);
    return val + 1;      /* a(n) = 1 + floor(n*(√2 – 1)) */
}

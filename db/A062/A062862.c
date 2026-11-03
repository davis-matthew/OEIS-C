/*
 * A062862  Number of ways n can be written as the sum of squares of
 *          consecutive numbers (possibly including squares of negative
 *          numbers).
 * https://oeis.org/A062862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as a sum of consecutive
 *            squares (the starting integer may be negative). The count is the
 *            number of integer pairs (m,k) with k≥1,m∈ℤ such that n = k*m^2 +
 *            m*k(k-1) + k(k-1)(2k-1)/6. This reduces to the quadratic
 *            condition k*(12n – k(k²–1)) must be divisible by...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square test for non‑negative 128‑bit values.
   Returns true if val is a perfect square, and stores the floor
   square root in *root when non‑NULL. */
static inline bool is_square128(__int128 val, __int128 *root)
{
    if (val < 0) return false;

    /* binary search for floor(sqrt(val)) within 0 … 2^63−1,
       which covers all 128‑bit numbers that may appear here. */
    __int128 low = 0, high = ( (__int128)1 << 63 ) - 1;
    while (low <= high) {
        __int128 mid = (low + high) >> 1;
        __int128 sq = mid * mid;
        if (sq == val) {
            if (root) *root = mid;
            return true;
        }
        if (sq < val) {
            low = mid + 1;
        } else {
            if (mid == 0) break;          /* avoid infinite loop */
            high = mid - 1;
        }
    }
    if (root) *root = high;   /* floor square root */
    return false;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; ; ++k) {
        /* left = 12*n, right = k*(k^2 – 1)  (all 128‑bit to avoid overflow) */
        __int128 left  = ( __int128)12 * ( __int128)n;
        __int128 kk    = ( __int128)k * ( __int128)k;   /* k^2          */
        __int128 right = ( __int128)k * (kk - 1);       /* k(k^2–1)     */

        if (left < right)    /* further k will only increase right */
            break;

        __int128 numerator = left - right;             /* >= 0          */
        __int128 denom     = (__int128)3 * ( __int128)k;/* > 0          */

        if (numerator % denom != 0)
            continue;                                 /* R not integer */

        __int128 R = numerator / denom;                /* integer          */

        __int128 s;
        if (!is_square128(R, &s))
            continue;                                 /* R not a square   */

        if (s == 0) {
            /* s = 0 gives a single solution if L (k) is odd. */
            if (((k - 1) & 1) == 0)
                count += 1;
        } else {
            /* two solutions (±s) if s and k-1 have same parity. */
            if (((s & 1) == ((k - 1) & 1)))
                count += 2;
        }
    }
    return count;
}

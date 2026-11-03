/*
 * A233327  Distance from 2^n to the nearest triangular number.
 * https://oeis.org/A233327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k+1) = 2^k (k ≥ 0). For even n > 0, let m =
 *            floor((sqrt(8·2^n + 1) - 1) / 2), Let T_m = m(m+1)/2, T_{m+1} =
 *            (m+1)(m+2)/2. Then a(n) = min(|2^n - T_m|, |T_{m+1} - 2^n|).
 *            a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 46 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Handle the first term explicitly.  */
    if (n == 0)
        return 0;                /* 2^0 = 1, nearest triangular = 1, distance = 0 */

    /* Odd indices: a(2*k+1) = 2^k.  */
    if (n & 1ULL)                /* n is odd */
        return pow2(n >> 1);     /* n/2 */

    /* Even indices: compute distance to nearest triangular number. */
    seq_t two_n = pow2(n);           /* 2^n */
    seq_t eight_two_n = two_n * (seq_t)8;  /* 8 * 2^n */
    seq_t val = eight_two_n + (seq_t)1;    /* 8*2^n + 1 */

    seq_t sqrt_val = floor_sqrt(val);       /* floor(sqrt(8*2^n + 1)) */
    seq_t m_candidate = sqrt_val - (seq_t)1;
    seq_t m = m_candidate / (seq_t)2;       /* floor(((sqrt(...) - 1) / 2)) */

    seq_t Tm   = (m * (m + (seq_t)1)) / (seq_t)2;
    seq_t Tm1  = ((m + (seq_t)1) * (m + (seq_t)2)) / (seq_t)2;

    seq_t diff1 = (two_n >= Tm) ? (two_n - Tm) : (Tm - two_n);
    seq_t diff2 = (Tm1 >= two_n) ? (Tm1 - two_n) : (two_n - Tm1);

    return (diff1 < diff2) ? diff1 : diff2;
}

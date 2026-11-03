/*
 * A059648  a(n) = [[(k^2)*n]-(k*[k*n])], where k = sqrt(2) and [] is the
 *          floor function.
 * https://oeis.org/A059648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 2*n - sqrt(2)*floor(sqrt(2)*n) ), an integer 0/1
 *            sequence
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9637 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    if (n == 0)                return 0;

    seq_t nn   = (seq_t)n;           /* convert index to seq_t */
    seq_t n2   = nn * nn;            /* n^2 */
    seq_t d    = n2 * (seq_t)2;      /* 2*n^2 */
    seq_t m    = floor_sqrt(d);      /* floor(n*sqrt(2)) */

    seq_t two_n_minus1 = (nn << 1);  /* 2*n */
    two_n_minus1--;                 /* 2*n - 1 */
    seq_t lhs = two_n_minus1 * two_n_minus1;   /* (2*n-1)^2 */
    seq_t rhs = m * m * (seq_t)2;    /* 2*m^2 */

    return (lhs >= rhs) ? 1 : 0;
}

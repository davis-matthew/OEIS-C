/*
 * A190242  a(n) = [n*u+n*v]-[n*u]-[n*v], where u=sqrt(2), v=sqrt(3), and
 *          []=floor.
 * https://oeis.org/A190242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*u + n*v) - floor(n*u) - floor(n*v) where u =
 *            sqrt(2) and v = sqrt(3)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 147 terms through n = 9355 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    /* compute floor(n*sqrt(2)) and floor(n*sqrt(3)) by integer square roots */
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;                     /* n^2  */
    seq_t au = floor_sqrt( (seq_t)2 * nn2 ); /* floor(n*sqrt(2)) */
    seq_t av = floor_sqrt( (seq_t)3 * nn2 ); /* floor(n*sqrt(3)) */

    /* T = floor(n*sqrt(2)) + floor(n*sqrt(3)) + 1                */
    seq_t T = au + av + (seq_t)1;

    /* C = T^2 - 5*n^2  (may be negative) */
    unsigned long long Tll = (unsigned long long)T;
    unsigned long long n2ll = (unsigned long long)nn2;
    long long C_val = (long long)(Tll * Tll) - 5LL * (long long)n2ll;

    /* a(n) = 1 iff C <= 0 or C^2 <= 24*n^4 */
    if (C_val <= 0)
        return (seq_t)1;

    __int128 C128 = (__int128)C_val;           /* signed 128‑bit */
    unsigned __int128 lhs = (unsigned __int128)C128 * (unsigned __int128)C128;
    unsigned __int128 rhs = (unsigned __int128)24 *
                           (unsigned __int128)n2ll * (unsigned __int128)n2ll;

    return (lhs <= rhs) ? (seq_t)1 : (seq_t)0;
}

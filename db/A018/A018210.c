/*
 * A018210  Alkane (or paraffin) numbers l(9,n).
 * https://oeis.org/A018210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = (n+1)(n+2)(n+3)^2(4n^2+6n+5)/90 a(2n-1) =
 *            n(n+1)(n+2)(n+3)(4n^2+6n+5)/90
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t compute_even(idx_t m)
{
    unsigned __int128 a = (unsigned __int128)(m + 1);
    unsigned __int128 b = (unsigned __int128)(m + 2);
    unsigned __int128 c = (unsigned __int128)(m + 3);
    unsigned __int128 t = (unsigned __int128)4 * m * m + (unsigned __int128)6 * m + 5;
    unsigned __int128 prod = a * b;
    prod *= c * c;
    prod *= t;
    prod /= 90;
    return (seq_t)prod;
}

static seq_t compute_odd(idx_t m)
{
    unsigned __int128 a = (unsigned __int128)m;
    unsigned __int128 b = (unsigned __int128)(m + 1);
    unsigned __int128 c = (unsigned __int128)(m + 2);
    unsigned __int128 d = (unsigned __int128)(m + 3);
    unsigned __int128 t = (unsigned __int128)4 * m * m + (unsigned __int128)6 * m + 5;
    unsigned __int128 prod = a * b;
    prod *= c;
    prod *= d;
    prod *= t;
    prod /= 90;
    return (seq_t)prod;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        idx_t m = n >> 1;
        return compute_even(m);
    } else {
        idx_t m = (n >> 1) + 1;
        return compute_odd(m);
    }
}

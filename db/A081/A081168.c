/*
 * A081168  Differences of Beatty sequence for square root of 10.
 * https://oeis.org/A081168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*sqrt(10)) - floor(n*sqrt(10))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 128‑bit value.
   Returns floor(sqrt(x)).  The result always fits in a seq_t
   because sqrt(10*n²) <= sqrt(10)*n < 4*n and n is small
   compared to the width of seq_t. */
static inline seq_t isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64);   /* sqrt(2¹²⁸‑1) < 2⁶⁴ */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)(lo - 1);
}

seq_t a(idx_t n)
{
    unsigned __int128 nt  = (unsigned __int128)n;      /* current index      */
    unsigned __int128 nt1 = nt + 1;                   /* next index         */

    /* 10*n*n and 10*(n+1)*(n+1) fit in 128 bits for the ranges
       we need to support (n up to at least 1e9). */
    unsigned __int128 sq_n  = nt * nt * (unsigned __int128)10;
    unsigned __int128 sq_n1 = nt1 * nt1 * (unsigned __int128)10;

    seq_t f_n  = isqrt128(sq_n);
    seq_t f_n1 = isqrt128(sq_n1);

    return f_n1 - f_n;   /* always 3 or 4 */
}

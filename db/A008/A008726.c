/*
 * A008726  Molien series 1/((1-x)^2*(1-x^8)) for 3-dimensional group [2,n] =
 *          *22n.
 * https://oeis.org/A008726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..n+8} floor(j/8) /* Closed form: let m = n+8; q
 *            = m/8; r = m%8; then a(n) = 4*q*(q-1) + (r+1)*q
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n + 8u;
    idx_t q = m / 8u;
    idx_t r = m % 8u;

    unsigned __int128 tmp = 4u * (unsigned __int128)q * (q - 1u)
                          + (unsigned __int128)(r + 1u) * q;
    return (seq_t)tmp;
}

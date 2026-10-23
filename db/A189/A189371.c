/*
 * A189371  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(2), t=sqrt(5).
 * https://oeis.org/A189371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/√2⌋ + ⌊n*√5/√2⌋
 * Range:     u8 n <= 5, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute n^2.  Use seq_t to keep enough room. */
    seq_t nn = (seq_t)n * (seq_t)n;

    /* floor( n / √2 ) = floor( sqrt( n^2 / 2 ) ) */
    seq_t x1 = nn / 2U;
    seq_t f1 = floor_sqrt(x1);

    /* floor( n*√5/√2 ) = floor( sqrt( (5*n^2) / 2 ) ) */
    seq_t x2 = (seq_t)5U * nn / 2U;
    seq_t f2 = floor_sqrt(x2);

    return (seq_t)n + f1 + f2;
}

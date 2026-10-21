/*
 * A070435  a(n) = n^2 mod 12, or alternately n^4 mod 12.
 * https://oeis.org/A070435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 12
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)(n % 12);   /* bring only the relevant part of n */
    return (m * m) % 12;         /* square and reduce modulo 12 */
}

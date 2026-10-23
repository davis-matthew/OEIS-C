/*
 * A130794  Periodic sequence with period 1,5,3.
 * https://oeis.org/A130794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 5 - 2 * mod(n+2,3)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* Compute (n+2) mod 3 safely without overflow */
    idx_t m = (n % 3 + 2) % 3;
    return (seq_t)5 - (seq_t)2 * (seq_t)m;
}

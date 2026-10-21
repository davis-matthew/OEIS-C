/*
 * A214066  a(n) = floor( (3/2)*floor(5*n/2) ).
 * https://oeis.org/A214066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3/2) * floor(5*n/2))
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(5*n/2) using integer division */
    seq_t t = (seq_t)5 * n / 2;
    /* Compute floor((3/2) * t) */
    return (seq_t)3 * t / 2;
}

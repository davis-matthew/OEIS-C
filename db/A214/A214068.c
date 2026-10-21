/*
 * A214068  a(n) = floor((3/2)*floor((3/2)*n)).
 * https://oeis.org/A214068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3/2)*floor((3/2)*n))
 * Range:     u8 n <= 113, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor((3*n)/2) first */
    seq_t t = ((seq_t)n * (seq_t)3) / (seq_t)2;
    /* then compute floor((3*t)/2) */
    return (t * (seq_t)3) / (seq_t)2;
}

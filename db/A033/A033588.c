/*
 * A033588  Duplicate of A014635.
 * https://oeis.org/A033588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 8*n^2 - 2*n (equivalently a(n) = 2*n*(4*n-1) )
 * Range:     i8 n <= 4, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the unsigned index to the sequence type before any arithmetic */
    seq_t N = (seq_t)n;
    return N * ( N*8 - 2 );
}

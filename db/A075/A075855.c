/*
 * A075855  Maximum number of black squares on an n X n chessboard (with a
 *          black square in at least one corner) that can be covered by a
 *          single path, traveling only to adjacent black squares.
 * https://oeis.org/A075855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ((n-1)^2)/2 + 1 if n is odd a(n) = (n^2 - n + 2)/2 if n
 *            is even
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t) n;
    if (n & 1) {                 /* odd n */
        seq_t t = N - 1;         /* n-1 is even */
        return (t * t) / 2 + 1;
    } else {                     /* even n */
        return (N * N - N + 2) / 2;
    }
}

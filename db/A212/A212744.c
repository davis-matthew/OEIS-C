/*
 * A212744  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          w=max{w,x,y,z}-min{w,x,y,z}; i.e., the range of (w,x,y,z) is its
 *          first term.
 * https://oeis.org/A212744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*n^3+24*n^2+24*n+8)/8 for n even a(n) =
 *            (15*n^3+21*n^2+21*n+7)/8 for n odd
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 713, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* promote index to seq_t */
    seq_t result;
    if ((n & 1) == 0) {          /* n is even */
        result = ((15 * N * N * N) + (24 * N * N) + (24 * N) + 8) / 8;
    } else {                     /* n is odd */
        result = ((15 * N * N * N) + (21 * N * N) + (21 * N) + 7) / 8;
    }
    return result;
}

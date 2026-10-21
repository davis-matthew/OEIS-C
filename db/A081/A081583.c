/*
 * A081583  Third row of Pascal-(1,2,1) array A081577.
 * https://oeis.org/A081583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2 + 9*n + 9*n^3)/2
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 815, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t nn2  = nn * nn;
    seq_t nn3  = nn2 * nn;
    seq_t num  = (seq_t)2 + (seq_t)9 * nn + (seq_t)9 * nn3;
    return num / (seq_t)2;
}

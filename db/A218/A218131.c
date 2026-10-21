/*
 * A218131  Number of length 8 primitive (=aperiodic or period 8) n-ary words.
 * https://oeis.org/A218131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 - n^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^4 and n^8 using seq_t arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t n2  = nn * nn;      /* n^2 */
    seq_t n4  = n2 * n2;      /* n^4 */
    seq_t n8  = n4 * n4;      /* n^8 */
    return n8 - n4;
}

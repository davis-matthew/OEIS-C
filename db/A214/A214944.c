/*
 * A214944  Number of squarefree words of length 5 in an (n+1)-ary alphabet.
 * https://oeis.org/A214944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + n^4 - 2*n^3 - n^2 + n
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;        /* cast index to the sequence type   */
    seq_t n2 = N * N;           /* n^2                                 */
    seq_t n3 = n2 * N;          /* n^3                                 */
    seq_t n4 = n3 * N;          /* n^4                                 */
    seq_t n5 = n4 * N;          /* n^5                                 */
    seq_t res = n5 + n4 - (n3 + n3) - n2 + N;
    return res;
}

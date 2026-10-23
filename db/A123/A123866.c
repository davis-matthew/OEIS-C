/*
 * A123866  a(n) = n^6 - 1.
 * https://oeis.org/A123866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A123866 (offset 1) */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* cast index to the sequence type */
    seq_t n2 = nn * nn;       /* n^2   */
    seq_t n4 = n2 * n2;       /* n^4   */
    seq_t n6 = n4 * n2;       /* n^6   */
    return n6 - (seq_t)1;     /* n^6 - 1 */
}

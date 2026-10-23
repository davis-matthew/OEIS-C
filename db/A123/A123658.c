/*
 * A123658  a(n) = 1 + n^4 + n^6 + n^9 + n^10.
 * https://oeis.org/A123658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n^4 + n^6 + n^9 + n^10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour for n<1 is irrelevant */
    seq_t sn = (seq_t)n;           /* cast index to sequence type */

    seq_t n2  = sn * sn;           /* n^2 */
    seq_t n4  = n2 * n2;           /* n^4 */
    seq_t n6  = n4 * n2;           /* n^6 */
    seq_t n8  = n4 * n4;           /* n^8  (needed for n^9 and n^10) */
    seq_t n9  = n8 * sn;           /* n^9 */
    seq_t n10 = n9 * sn;           /* n^10 */

    return (seq_t)1 + n4 + n6 + n9 + n10;
}

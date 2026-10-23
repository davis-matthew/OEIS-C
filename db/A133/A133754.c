/*
 * A133754  a(n) = n^5 - n^3.
 * https://oeis.org/A133754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^5 - n^3
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2   = n_seq * n_seq;      /* n^2   */
    seq_t n3   = n2   * n_seq;      /* n^3   */
    seq_t n5   = n3   * n2;         /* n^5   */
    return n5 - n3;
}

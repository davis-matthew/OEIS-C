/*
 * A082801  Smallest multiple of 6 beginning with n.
 * https://oeis.org/A082801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n if n mod 6 = 0 10*n + 2*(n mod 3) otherwise which
 *            yields the smallest multiple of 6 starting with the decimal
 *            representation of n
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* convert index to sequence type   */
    if ((n_seq % (seq_t)6) == 0)      /* already a multiple of 6         */
        return n_seq;

    /* otherwise add the smallest number to make it a multiple of 6 */
    seq_t mod3 = n_seq % (seq_t)3;
    return (seq_t)10 * n_seq + (seq_t)2 * mod3;
}

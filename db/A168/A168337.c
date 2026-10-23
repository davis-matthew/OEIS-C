/*
 * A168337  a(n) = 1 + 7*floor(n/2).
 * https://oeis.org/A168337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 + 7*floor(n/2)
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid type promotion issues. */
    seq_t nn = (seq_t)n;

    /* Integer division implements floor(n/2). */
    return 1 + 7 * (nn / 2);
}

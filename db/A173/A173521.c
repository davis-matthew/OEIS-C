/*
 * A173521  a(n) is the concatenation of n 1's, 0, n 1's and 0.
 * https://oeis.org/A173521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the concatenation of n 1's, 0, n 1's, and 0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 gives the first term 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t x = (seq_t)0;

    /* append n ones */
    for (idx_t i = (idx_t)0; i < n; ++i)
        x = x * (seq_t)10 + (seq_t)1;

    /* append the trailing zero after the first block of ones */
    x *= (seq_t)10;

    /* append another n ones */
    for (idx_t i = (idx_t)0; i < n; ++i)
        x = x * (seq_t)10 + (seq_t)1;

    /* final trailing zero */
    x *= (seq_t)10;

    return x;
}

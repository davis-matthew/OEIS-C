/*
 * A180359  a(n) = n^9 + 9*n.
 * https://oeis.org/A180359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^9 + 9*n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* integer value of n in seq_t type */
    seq_t pow = sn;               /* will hold n^k, start with n^1 */

    /* Compute n^9 by successive multiplications */
    for (int i = 1; i < 9; ++i)
        pow *= sn;

    /* Return n^9 + 9*n */
    return pow + (seq_t)9 * sn;
}

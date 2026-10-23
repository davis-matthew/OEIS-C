/*
 * A141060  Fourth quadrisection of Jacobsthal numbers A001045:
 *          a(n)=16a(n-1)-5.
 * https://oeis.org/A141060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 16*a(n-1) - 5, a(0) = 3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first term directly. */
    if (n == 0)
        return (seq_t)3;

    /* Compute the sequence iteratively. */
    seq_t val = (seq_t)3;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * (seq_t)16 - (seq_t)5;
    }
    return val;
}

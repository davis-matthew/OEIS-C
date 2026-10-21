/*
 * A069140  a(n) = (4*n-1)*4*n*(4*n+1).
 * https://oeis.org/A069140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n-1)*4*n*(4*n+1)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 406, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0; for n > 0 computation is safe because 4*n > 0 */
    if (n == 0)
        return 0;

    seq_t m = (seq_t)n;
    seq_t four_n = m * 4;
    seq_t term = (four_n - 1) * four_n * (four_n + 1);
    return term;
}

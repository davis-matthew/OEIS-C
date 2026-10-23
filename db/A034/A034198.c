/*
 * A034198  Number of binary codes (not necessarily linear) of length n with 3
 *          words.
 * https://oeis.org/A034198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(2*n^2 + 21*n - 6)/72)
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* offset starts at n = 1 */

    seq_t nn = (seq_t)n;

    /* compute 2*n^2 + 21*n - 6 */
    seq_t temp = 2 * nn * nn + 21 * nn - 6;

    /* compute n*(2*n^2 + 21*n - 6) */
    seq_t num = nn * temp;

    /* floor division by 72 */
    return num / 72;
}

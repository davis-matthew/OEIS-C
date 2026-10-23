/*
 * A011902  a(n) = floor( n*(n-1)*(n-2)/20 ).
 * https://oeis.org/A011902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)/20 )
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1667, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)                 /* n = 0,1,2 gives 0 */
        return 0;
    seq_t nn = (seq_t)n;
    /* compute (n*(n-1)*(n-2)) / 20 using seq_t arithmetic only */
    seq_t num = nn * (nn - 1) * (nn - 2);
    return num / 20;
}

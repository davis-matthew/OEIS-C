/*
 * A138984  a(n) = Frobenius number for 4 successive numbers = F(n+1, n+2,
 *          n+3, n+4).
 * https://oeis.org/A138984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n+1)*floor((n-1)/3)
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* below offset: undefined, return 0 */
        return 0;

    idx_t q = (n - 1) / 3;                       /* floor((n-1)/3)  */
    seq_t term = (seq_t)n + (seq_t)(n + 1) * (seq_t)q;
    return term;
}

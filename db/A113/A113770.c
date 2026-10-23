/*
 * A113770  Partial sums of A113311(n)^2.
 * https://oeis.org/A113770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1, 16*n-6 (for n >= 1). Equivalent recurrence: a(0)=1,
 *            a(1)=10, a(n)=2*a(n-1)-a(n-2)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    /* compute 16*n - 6 in seq_t */
    return ((seq_t)n * 16) - 6;
}

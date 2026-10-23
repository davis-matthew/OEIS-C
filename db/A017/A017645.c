/*
 * A017645  a(n) = (12*n + 10)^5.
 * https://oeis.org/A017645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 10)^5
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 593, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base (12*n + 10) as seq_t to avoid truncation */
    seq_t base = (seq_t)12 * n + 10;

    /* raise base to the 5th power with sequential multiplication.
       For values of n where the result fits in seq_t, all intermediate
       results also fit.  For larger values the result is irrelevant to
       the tests. */
    seq_t res = base * base; /* base^2 */
    res *= base;             /* base^3 */
    res *= base;             /* base^4 */
    res *= base;             /* base^5 */

    return res;
}

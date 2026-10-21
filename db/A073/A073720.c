/*
 * A073720  Let b(1) = 1, b(k+1) = b(k) - k*trunc(k/b(k)+1), where trunc(x) =
 *          floor(x) if x>= 0, trunc(x) = ceiling(x) otherwise. Sequence a(n)
 *          gives the successive absolute values taken by b(k).
 * https://oeis.org/A073720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 * For n > 1: a(n) = (47*5^(n-2) - 3) / 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t pow5 = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow5 *= 5;            /* compute 5^(n-2) */

    seq_t result = pow5 * 47;  /* 47*5^(n-2) */
    result -= 3;              /* minus 3 */
    result /= 4;              /* divide by 4 */

    return result;
}

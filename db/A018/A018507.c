/*
 * A018507  Divisors of 536.
 * https://oeis.org/A018507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 536 in ascending order. 536 = 2^3 * 67, so
 *            the divisors are 1,2,4,8,67,134,268,536. For n <= 4: a(n) = 1 <<
 *            (n-1) For 5 <= n <= 8: a(n) = 67 << (n-5) For n > 8: a(n) = 0
 *            (undefined)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)          /* not defined for n < 1, return 0 */
        return 0;
    else if (n <= 4)    /* 1, 2, 4, 8 */
        return ((seq_t)1) << (unsigned)(n - 1);
    else if (n <= 8)    /* 67, 134, 268, 536 */
        return ((seq_t)67) << (unsigned)(n - 5);
    else                /* no terms beyond the 8 divisors of 536 */
        return 0;
}

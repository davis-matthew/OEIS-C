/*
 * A088924  Number of "9ish numbers" with n digits.
 * https://oeis.org/A088924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 19*a(n-1) - 90*a(n-2) with initial values a(1) = 1, a(2)
 *            = 18 (equivalent to a(n) = 9*10^(n-1) - 8*9^(n-1) )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* offset is 1; n=0 is undefined */
    if (n == 1)
        return 1;
    if (n == 2)
        return 18;

    seq_t prev2 = 1;            /* a(n-2) */
    seq_t prev1 = 18;           /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)19 - prev2 * (seq_t)90;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

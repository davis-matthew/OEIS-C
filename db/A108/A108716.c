/*
 * A108716  a(n) = tan(Pi/14)^(-2n) + tan(3*Pi/14)^(-2n) + tan(5*Pi/14)^(-2n).
 * https://oeis.org/A108716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 35*a(n-2) + 7*a(n-3), with initial values
 *            a(0)=3, a(1)=21, a(2)=371. This recurrence follows from the
 *            roots tan(Pi/14)^2, tan(3Pi/14)^2, tan(5Pi/14)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 3;
    if (n == 1) return 21;
    if (n == 2) return 371;

    /* dp[0] = a(n-3), dp[1] = a(n-2), dp[2] = a(n-1) */
    seq_t dp[3];
    dp[0] = 3;
    dp[1] = 21;
    dp[2] = 371;

    const seq_t c21 = 21;
    const seq_t c35 = 35;
    const seq_t c7  = 7;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = c21 * dp[2] + c7 * dp[0];
        next = next - c35 * dp[1];
        dp[0] = dp[1];
        dp[1] = dp[2];
        dp[2] = next;
    }

    return dp[2];
}

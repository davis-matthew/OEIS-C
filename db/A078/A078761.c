/*
 * A078761  Sum of the digits of all n-digit numbers.
 * https://oeis.org/A078761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 45*n*10^(n-1) - 45*(n-1)*10^(n-2) (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* offset starts at 1; zero is not a valid input */
        return (seq_t)0;

    /* Compute 10^(n-1) in seq_t */
    seq_t pow10_nm1 = 1;
    for (idx_t i = 1; i <= n - 1; ++i)
        pow10_nm1 *= (seq_t)10;

    /* 10^(n-2) is just 10^(n-1) divided by 10 */
    seq_t pow10_nm2 = pow10_nm1 / (seq_t)10;

    /* a(n) = 45*n*10^(n-1) - 45*(n-1)*10^(n-2) */
    seq_t termA = (seq_t)45 * (seq_t)n * pow10_nm1;
    seq_t termB = (seq_t)45 * (seq_t)(n - 1) * pow10_nm2;

    return termA - termB;
}

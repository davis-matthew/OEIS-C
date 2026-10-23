/*
 * A024382  a(n) = n-th elementary symmetric function of the first n+1
 *          positive integers congruent to 1 mod 4.
 * https://oeis.org/A024382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n-2)*a(n-1) - (4*n-3)^2*a(n-2) with initial values
 *            a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = prev1 * (seq_t)(8 * i - 2);
        idx_t m = 4 * i - 3;
        seq_t term2 = prev2 * (seq_t)(m * m);
        seq_t cur   = term1 - term2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

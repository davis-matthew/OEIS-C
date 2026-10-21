/*
 * A097737  Chebyshev U(n,x) polynomial evaluated at x=163.
 * https://oeis.org/A097737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*163*a(n-1) - a(n-2), a(0)=1, a(1)=326
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 326;

    seq_t prev0 = 1;   /* a(n-2) */
    seq_t prev1 = 326; /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * 326 - prev0;
        prev0 = prev1;
        prev1 = cur;
    }
    return prev1;
}

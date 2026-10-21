/*
 * A098261  Chebyshev polynomials S(n,627) + S(n-1,627) with Diophantine
 *          property.
 * https://oeis.org/A098261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 627*a(n-1) - a(n-2) (n>1); Initial values: a(0)=1,
 *            a(1)=628
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return 1;
    if (n == 1) return 628;

    /* Iterate the linear recurrence */
    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 628; /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 627 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

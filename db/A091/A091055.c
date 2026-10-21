/*
 * A091055  Expansion of x*(1-2*x)/((1-x)*(1+2*x)*(1-6*x)).
 * https://oeis.org/A091055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*6^n - 5*(-2)^n + 2)/30 for n ≥ 0 Recurrence: a(0)=0,
 *            a(1)=1, a(2)=3, a(n) = 5*a(n-1) + 8*a(n-2) - 12*a(n-3) for n ≥ 3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;

    seq_t prev3 = 0;   /* a(n-3)  */
    seq_t prev2 = 1;   /* a(n-2)  */
    seq_t prev1 = 3;   /* a(n-1)  */
    seq_t cur = 0;     /* a(n)    */

    for (idx_t i = 3; i <= n; ++i) {
        cur = 5 * prev1 + 8 * prev2 - 12 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}

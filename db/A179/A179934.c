/*
 * A179934  Expansion of x*(4+5*x-13*x^2-x^3+x^4) / ( (1-x)*(1-10*x^2+x^4) ).
 * https://oeis.org/A179934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+4) = 10*a(n+2) - a(n) - 4 Initial values: a(1)=4, a(2)=9,
 *            a(3)=36, a(4)=85
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 4;
    if (n == 2) return 9;
    if (n == 3) return 36;
    if (n == 4) return 85;

    /* sliding window: prev4 = a(i-4), prev3 = a(i-3),
       prev2 = a(i-2), prev1 = a(i-1) */
    seq_t prev4 = 4;   /* a(n-4) */
    seq_t prev3 = 9;   /* a(n-3) */
    seq_t prev2 = 36;  /* a(n-2) */
    seq_t prev1 = 85;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = 10 * prev2 - prev4 - 4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;  /* a(n) */
}

/*
 * A189317  Expansion of 5*(1-6*x+x^2)/(1-10*x+5*x^2).
 * https://oeis.org/A189317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)-5*a(n-2), a(0)=5, a(1)=20, a(2)=180
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)180;

    seq_t prev2 = (seq_t)20;   /* a(1) */
    seq_t prev1 = (seq_t)180;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = (seq_t)10 * prev1 - (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

/*
 * A100295  Expansion of g.f. x*(1-x)/(1-4*x-2*x^2+x^3).
 * https://oeis.org/A100295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - a(n-3); a(1)=1, a(2)=3, a(3)=14
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)14;

    seq_t a_prev2 = (seq_t)3;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)14;  /* a(n-1) */
    seq_t a_prev3 = (seq_t)1;   /* a(n-3) */
    seq_t current = 0;

    for (idx_t i = 4; i <= n; ++i) {
        current = 4 * a_prev1 + 2 * a_prev2 - a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = current;
    }
    return current;
}

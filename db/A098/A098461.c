/*
 * A098461  Expansion of E.g.f.: 1/sqrt(1-2*x-3*x^2).
 * https://oeis.org/A098461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n - 1) * a(n-1) + 3*(n-1)^2 * a(n-2) with a(0) = 1,
 *            a(1) = 1. This uses the D‑finite recurrence given in the OEIS
 *            entry
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)1;   /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* (2*i - 1) and 3*(i-1)*(i-1) fit comfortably in 64 bits */
        seq_t term1 = a_prev1 * (seq_t)(2*i - 1);
        seq_t term2 = a_prev2 * (seq_t)(3*(i-1)*(i-1));

        a_cur = term1 + term2;

        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur;
}

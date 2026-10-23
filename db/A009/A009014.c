/*
 * A009014  Expansion of E.g.f.: (1 + x)/(1 + x + x^2/2).
 * https://oeis.org/A009014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n*a(n-1)-n*(n-1)/2*a(n-2), a(0)=1, a(1)=0
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)0;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* term1 = i * a(i-1) */
        seq_t factor1 = (seq_t)i * prev1;

        /* t = i*(i-1)/2  (computed in idx_t to avoid overflow in 64‑bit) */
        idx_t t = (i * (i - 1)) / 2;
        seq_t factor2 = (seq_t)t * prev2;

        curr = -factor1 - factor2;

        /* shift for next step */
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}

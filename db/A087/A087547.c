/*
 * A087547  a(n) = n!*2^(n+1) * (Integral_{x = 0..1} 1/(1+x^2)^(n+1) dx) -
 *          Pi*(2*n)!/(2^(n+1)*n!).
 * https://oeis.org/A087547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n-1)*a(n-1) + (n-1)! with a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t fact = (seq_t)1;  /* (i-1)! ; starts as 0! */
    seq_t a_prev = (seq_t)0; /* a(0) */

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* fact now holds (i-1)! */
        seq_t fact_prev = fact;

        seq_t coeff = (seq_t)(2*(seq_t)i - (seq_t)1);
        seq_t a_cur   = coeff * a_prev + fact_prev;

        a_prev = a_cur;
        fact  *= (seq_t)i;   /* advance to i! for next iteration */
    }

    return a_prev;  /* a(n) */
}
